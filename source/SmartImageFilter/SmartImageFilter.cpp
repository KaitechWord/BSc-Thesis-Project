#include "SmartImageFilter.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>
#include <unordered_map>

static const auto minSingleTextFile = std::string(ROOT_DIR) + "/output/SmartMinSingle.txt";
static const auto minMultiTextFile = std::string(ROOT_DIR) + "/output/SmartMinMulti.txt";
static const auto maxSingleTextFile = std::string(ROOT_DIR) + "/output/SmartMaxSingle.txt";
static const auto maxMultiTextFile = std::string(ROOT_DIR) + "/output/SmartMaxMulti.txt";

SmartImageFilter::SmartImageFilter(int threadNum, AlgorithmType algType, int maskSize)
	: ImageFilter(threadNum, algType, maskSize)
{}

void SmartImageFilter::apply(cv::Mat& image) {
	this->data = image;
	auto rows = image.rows;
	auto cols = image.cols;
	auto pixelsNum = rows * cols;
	auto threadsNum = this->threadNum;
	if (threadsNum > pixelsNum) {
		std::cout << "The number of threads is bigger than the size of the image. Setting number of threads to size.\n";
		threadsNum = pixelsNum;
	}
	auto sizeOfOneThread = pixelsNum / threadsNum;
	auto remainder = pixelsNum % threadsNum;

	std::vector<std::thread> threads;
	cv::Mat newImage(rows, cols, image.type());
	auto start = std::chrono::high_resolution_clock::now();
	for (auto i = 0; i < threadsNum; ++i) {
		auto firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		auto lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		threads.emplace_back(std::thread(&SmartImageFilter::filter, this, std::ref(newImage), firstIndex, lastIndex));
	}
	for (auto& thread : threads)
		thread.join();
	auto end = std::chrono::high_resolution_clock::now();
	std::cout << "Smart " << (this->algType == AlgorithmType::MIN ? "min." : "max.") << " image " << (threadsNum == 1 ? "(single-threaded)" : "(multi-threaded)") << " filter execution time : " << std::chrono::duration<double, std::milli>(end - start).count() << "ms.\n";
	const auto execTime = std::chrono::duration<double, std::milli>(end - start).count();
	const auto textFile = this->algType == AlgorithmType::MIN ? (threadsNum == 1 ? minSingleTextFile : minMultiTextFile) : (threadsNum == 1 ? maxSingleTextFile : maxMultiTextFile);
	std::ofstream outfile;
	outfile.open(textFile, std::ios_base::app);
	if (outfile.is_open())
		outfile << execTime << "\n";
	else
		std::cerr << "File: " << textFile << " did not open successfully." << "\n";
	for (auto i = 0; i < threadsNum; ++i) {
		auto firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		auto lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		auto partOfimageIndex = 0;
		auto colSize = this->data.cols;
		for (auto j = firstIndex; j <= lastIndex; ++j) {
			image.at<uchar>(j / colSize, j % colSize) = newImage.at<uchar>(j / colSize, j % colSize);
		}
	}
	outfile.close();
}

int SmartImageFilter::getIndex( const Indices& indices, int left, int right, int row ){
	auto relativeLeft = left - indices.firstMask.left;
	auto relativeRight = right - indices.firstMask.left;
	auto relativeRow = row - indices.top;
	return  this->maskSize * 2*this->maskSize * relativeLeft + this->maskSize * relativeRight + relativeRow;
}

void SmartImageFilter::calculatePrefixes( const Indices& indices, Precalculations& precalculations, int row )
{
	auto prefixRightIndex = indices.firstMask.right;
	precalculations[ getIndex( indices, prefixRightIndex, prefixRightIndex, row ) ] = this->data.at<uchar>(row, prefixRightIndex);
	for (auto prefixLeftIndex = prefixRightIndex - 1; prefixLeftIndex >= indices.firstMask.left; --prefixLeftIndex) {
		auto freshValue = this->data.at<uchar>(row, prefixLeftIndex);
		auto previousPrefixValue = precalculations[ getIndex( indices, (prefixLeftIndex + 1), prefixRightIndex, row ) ];
		precalculations[ getIndex( indices, prefixLeftIndex, prefixRightIndex, row ) ] = this->compare(freshValue, previousPrefixValue) ? freshValue : previousPrefixValue;
	}
}

void SmartImageFilter::calculateSuffixes( const Indices& indices, Precalculations& precalculations, int row )
{
	auto suffixLeftIndex = indices.secondMask.left; 
	precalculations[ getIndex( indices, suffixLeftIndex, suffixLeftIndex, row ) ] = this->data.at<uchar>(row, suffixLeftIndex);
	for (auto suffixRightIndex = suffixLeftIndex + 1; suffixRightIndex <= indices.secondMask.right; ++suffixRightIndex) {
		auto freshValue = this->data.at<uchar>(row, suffixRightIndex);
		auto previousSuffixValue = precalculations[ getIndex( indices, suffixLeftIndex, suffixRightIndex - 1, row ) ];
		precalculations[ getIndex( indices, suffixLeftIndex, suffixRightIndex, row ) ] = this->compare(freshValue, previousSuffixValue) ? freshValue : previousSuffixValue;
	}
}

void SmartImageFilter::precalculate( const Indices& indices, Precalculations& precalculations )
{
	for (auto row = indices.top; row <= indices.bot; ++row) {
		calculatePrefixes( indices, precalculations, row );
		calculateSuffixes( indices, precalculations, row );
	}
}

void SmartImageFilter::setPrefixOnlyMaskExtremum( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations )
{
	auto extremum = precalculations[ getIndex(indices, indices.firstMask.left, indices.firstMask.right, indices.top) ];
	for (auto row = indices.top + 1; row <= indices.bot; ++row) {
		auto currentValue = precalculations[ getIndex( indices, indices.firstMask.left, indices.firstMask.right, row ) ];
		if (this->compare(currentValue, extremum)) {
			extremum = currentValue;
		}
	}
	newImage.at<uchar>(indices.row, indices.firstMask.center) = extremum;
}

void SmartImageFilter::setAffixMixMaskExtrema( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations )
{
	auto maskOneHalfLength = static_cast<int>( std::floor(this->maskSize / 2) );
	for (auto affixMixMaskCenterIndex = indices.firstMask.center + 1; affixMixMaskCenterIndex <= indices.secondMask.center; ++affixMixMaskCenterIndex) {
		
		auto currentMaskLeftIndex = std::clamp(affixMixMaskCenterIndex - maskOneHalfLength, indices.firstMask.left, indices.firstMask.right);
		auto currentMaskRightIndex = std::clamp(affixMixMaskCenterIndex + maskOneHalfLength, indices.secondMask.left, indices.secondMask.right);

		auto prefixPartOfMaskExtremum = precalculations[ getIndex( indices, currentMaskLeftIndex, indices.firstMask.right, indices.top ) ];
		auto suffixPartOfMaskExtremum = precalculations[ getIndex( indices, indices.secondMask.left, currentMaskRightIndex, indices.top ) ];

		auto extremum = this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum) ? prefixPartOfMaskExtremum : suffixPartOfMaskExtremum ;
		
		//Iterating from top row of mask up to bottom row of mask
		for (auto row = indices.top + 1; row <= indices.bot; ++row) {
			prefixPartOfMaskExtremum = precalculations[ getIndex( indices, currentMaskLeftIndex, indices.firstMask.right, row ) ];
			suffixPartOfMaskExtremum = precalculations[ getIndex( indices, indices.secondMask.left, currentMaskRightIndex, row ) ];

			if (this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum)) {
				if (this->compare(prefixPartOfMaskExtremum, extremum)) {
					extremum = prefixPartOfMaskExtremum;
				}
			}
			else {
				if (this->compare(suffixPartOfMaskExtremum, extremum)) {
					extremum = suffixPartOfMaskExtremum;
				}
			}
		}
		newImage.at<uchar>(indices.row, affixMixMaskCenterIndex) = extremum;
	}
}

void SmartImageFilter::setSuffixOnlyMaskExtremum( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations )
{
	auto extremum = precalculations[ getIndex( indices, indices.secondMask.left, indices.secondMask.right, indices.top ) ];
	for (auto row = indices.top + 1; row <= indices.bot; ++row) {
		auto currentValue = precalculations[ getIndex( indices, indices.secondMask.left, indices.secondMask.right, row ) ];
		if (this->compare(currentValue, extremum)) {
			extremum = currentValue;
		}
	}
	newImage.at<uchar>(indices.row, indices.secondMask.center) = extremum;
}

void SmartImageFilter::setExtrema( cv::Mat& newImage, const Indices& indices, Precalculations& precalculations )
{
	//Finding best value for left-first index from first mask (it is separate case - we need to only use prefixes from first mask)
		//Iterating from top row of mask up to bottom row of mask
		setPrefixOnlyMaskExtremum( newImage, indices, precalculations );
		//Finding best value for indices that are mix of prefixes from first mask and postfixes from second mask
		setAffixMixMaskExtrema( newImage, indices, precalculations );
		//Finding best value for last index from second mask (it is separate case - we need to only use postfixes from second mask)
		//Iterating from top row of mask up to bottom row of mask
		setSuffixOnlyMaskExtremum( newImage, indices, precalculations );
}

void SmartImageFilter::updateRowColumnAndIndex( int& row, int& column, int& index )
{
	auto valueToIterate = this->maskSize + 1;
	column += valueToIterate;  // Move to next mask position

	if (column >= this->data.cols) {  // Check if we need to transition to a new row
		row++;
		column = 0;
		index = row * this->data.cols;  // Move to the first column of the new row
	}
	else {
		index += valueToIterate;
	}
}

void SmartImageFilter::updateIndices( Indices& indices, int row, int column ){
	auto maskOneHalfLength = static_cast<int>(std::floor(this->maskSize / 2));
	indices.firstMask.center = column;
	indices.firstMask.left = std::max(column - maskOneHalfLength, 0);
	indices.firstMask.right = std::min(column + maskOneHalfLength, this->data.cols - 1);

	indices.secondMask.center = std::clamp(column + this->maskSize, 0, this->data.cols - 1);;
	indices.secondMask.left = std::max(indices.secondMask.center - maskOneHalfLength, 0);
	indices.secondMask.right = std::min(indices.secondMask.center + maskOneHalfLength, this->data.cols - 1);
	
	indices.top = std::max(row - maskOneHalfLength, 0);
	indices.bot = std::min(row + maskOneHalfLength, this->data.rows - 1);
	indices.row = row;
}

void SmartImageFilter::filter(cv::Mat& newImage, int firstIndex, int lastIndex) {
	//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	// auto maskOneHalfLength = this->maskSize / 2;
	//Starting value is set in base class in regards to algType
	// auto targetValue = this->startingValue;

	//First [) is first index of prefix/suffix, second [) is last index of prefix/suffix, [) is row	
	//An affix is a general term for a linguistic element added to a word, which includes:
    // Prefix (before the root)
    // Suffix (after the root)
    // Infix (inside the root, though rare in English)
	Precalculations affixesPrecalculations;
	// Flatten array of 3 dimensions - startIndexOfMask, endIndexOfMask, row
	//(  )
	affixesPrecalculations.reserve( this->maskSize * 2*this->maskSize * 2*this->maskSize + this->maskSize * 2*this->maskSize + this->maskSize );

	auto row = firstIndex / this->data.cols;
	auto column = firstIndex - (row * this->data.cols);
	auto indices = Indices{};
	// "i" is always the centre of first mask
	for (auto i = firstIndex; i <= lastIndex; ) {
		updateIndices(indices, row, column);

		precalculate( indices, affixesPrecalculations );
		setExtrema( newImage, indices, affixesPrecalculations );

		updateRowColumnAndIndex( row, column , i);
		affixesPrecalculations.clear();
	}
}