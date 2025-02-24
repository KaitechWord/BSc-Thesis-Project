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

int SmartImageFilter::getIndexPrefix( const Indices& indices, int left, int right, int row ){
	auto relativeLeft = left;
	auto relativeRight = right;
	auto relativeRow = row - indices.top;
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight + relativeRow;
}

int SmartImageFilter::getIndexSuffix( const Indices& indices, int left, int right, int row ){
	auto relativeLeft = left;
	auto relativeRight = right;
	auto relativeRow = row - indices.top;
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight + relativeRow;
}

void SmartImageFilter::calculatePrefixes( const Indices& indices, Precalculation& precalculation, int row )
{
	auto prefixRightIndex = indices.firstMask.right;
	precalculation[ getIndexPrefix( indices, prefixRightIndex, prefixRightIndex, row ) ] = this->data.at<uchar>(row, prefixRightIndex);
	for (auto prefixLeftIndex = prefixRightIndex - 1; prefixLeftIndex >= indices.firstMask.left; --prefixLeftIndex) {
		auto freshValue = this->data.at<uchar>(row, prefixLeftIndex);
		auto previousPrefixValue = precalculation[ getIndexPrefix( indices, (prefixLeftIndex + 1), prefixRightIndex, row ) ];
		precalculation[ getIndexPrefix( indices, prefixLeftIndex, prefixRightIndex, row ) ] = this->compare(freshValue, previousPrefixValue) ? freshValue : previousPrefixValue;
	}
}

void SmartImageFilter::calculateSuffixes( const Indices& indices, Precalculation& precalculation, int row )
{
	auto suffixLeftIndex = indices.secondMask.left; 
	precalculation[ getIndexSuffix( indices, suffixLeftIndex, suffixLeftIndex, row ) ] = this->data.at<uchar>(row, suffixLeftIndex);
	for (auto suffixRightIndex = suffixLeftIndex + 1; suffixRightIndex <= indices.secondMask.right; ++suffixRightIndex) {
		auto freshValue = this->data.at<uchar>(row, suffixRightIndex);
		auto previousSuffixValue = precalculation[ getIndexSuffix( indices, suffixLeftIndex, suffixRightIndex - 1, row ) ];
		precalculation[ getIndexSuffix( indices, suffixLeftIndex, suffixRightIndex, row ) ] = this->compare(freshValue, previousSuffixValue) ? freshValue : previousSuffixValue;
	}
}

void SmartImageFilter::precalculate( const Indices& indices, Precalculations& precalculations )
{
	for (auto row = indices.top; row <= indices.bot; ++row) {
		calculatePrefixes( indices, precalculations.prefixes, row );
		calculateSuffixes( indices, precalculations.suffixes, row );
	}
}

void SmartImageFilter::calculateFreshPrefixes( const Indices& indices, Precalculation& precalculation, int row )
{
	auto prefixRightIndex = indices.firstMask.right;
	precalculation[ getIndexPrefixFromRowToBeOverriden( indices, prefixRightIndex, prefixRightIndex ) ] = this->data.at<uchar>(row, prefixRightIndex);
	for (auto prefixLeftIndex = prefixRightIndex - 1; prefixLeftIndex >= indices.firstMask.left; --prefixLeftIndex) {
		auto freshValue = this->data.at<uchar>(row, prefixLeftIndex);
		auto previousPrefixValue = precalculation[ getIndexPrefixFromRowToBeOverriden( indices, (prefixLeftIndex + 1), prefixRightIndex ) ];
		precalculation[ getIndexPrefixFromRowToBeOverriden( indices, prefixLeftIndex, prefixRightIndex ) ] = this->compare(freshValue, previousPrefixValue) ? freshValue : previousPrefixValue;
	}
}

void SmartImageFilter::calculateFreshSuffixes( const Indices& indices, Precalculation& precalculation, int row )
{
	auto suffixLeftIndex = indices.secondMask.left; 
	precalculation[ getIndexSuffixFromRowToBeOverriden( indices, suffixLeftIndex, suffixLeftIndex ) ] = this->data.at<uchar>(row, suffixLeftIndex);
	for (auto suffixRightIndex = suffixLeftIndex + 1; suffixRightIndex <= indices.secondMask.right; ++suffixRightIndex) {
		auto freshValue = this->data.at<uchar>(row, suffixRightIndex);
		auto previousSuffixValue = precalculation[ getIndexSuffixFromRowToBeOverriden( indices, suffixLeftIndex, suffixRightIndex - 1 ) ];
		precalculation[ getIndexSuffixFromRowToBeOverriden( indices, suffixLeftIndex, suffixRightIndex ) ] = this->compare(freshValue, previousSuffixValue) ? freshValue : previousSuffixValue;
	}
}

void SmartImageFilter::precalculateFreshRow( const Indices& indices, Precalculations& precalculations )
{
	calculateFreshPrefixes( indices, precalculations.prefixes, indices.bot );
	calculateFreshSuffixes( indices, precalculations.suffixes, indices.bot );
}

void SmartImageFilter::setPrefixOnlyMaskExtremum( cv::Mat& newImage, Indices& indices, Precalculation& precalculation )
{
	auto extremum = precalculation[ getIndexPrefix(indices, indices.firstMask.left, indices.firstMask.right, indices.top) ];
	auto extremumRow = indices.top;
	for (auto row = indices.top + 1; row <= indices.bot; ++row) {
		auto currentValue = precalculation[ getIndexPrefix( indices, indices.firstMask.left, indices.firstMask.right, row ) ];
		if (this->compare(currentValue, extremum)) {
			extremum = currentValue;
			extremumRow = row;
		}
	}
	newImage.at<uchar>(indices.row, indices.firstMask.center) = extremum;
	indices.lastExtremaRow[ indices.firstMask.center ] = extremumRow;
}

void SmartImageFilter::setAffixMixMaskExtrema( cv::Mat& newImage, Indices& indices, Precalculations& precalculations )
{
	auto maskOneHalfLength = static_cast<int>( std::floor(this->maskSize / 2) );
	for (auto affixMixMaskCenterIndex = indices.firstMask.center + 1; affixMixMaskCenterIndex < indices.secondMask.center; ++affixMixMaskCenterIndex) {
		
		auto currentMaskLeftIndex = std::clamp(affixMixMaskCenterIndex - maskOneHalfLength, indices.firstMask.left, indices.firstMask.right);
		auto currentMaskRightIndex = std::clamp(affixMixMaskCenterIndex + maskOneHalfLength, indices.secondMask.left, indices.secondMask.right);

		auto prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefix( indices, currentMaskLeftIndex, indices.firstMask.right, indices.top ) ];
		auto suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffix( indices, indices.secondMask.left, currentMaskRightIndex, indices.top ) ];

		auto extremum = this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum) ? prefixPartOfMaskExtremum : suffixPartOfMaskExtremum ;
		auto  extremumRow= indices.top;
		
		//Iterating from top row of mask up to bottom row of mask
		for (auto row = indices.top + 1; row <= indices.bot; ++row) {
			prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefix( indices, currentMaskLeftIndex, indices.firstMask.right, row ) ];
			suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffix( indices, indices.secondMask.left, currentMaskRightIndex, row ) ];

			if (this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum)) {
				if (this->compare(prefixPartOfMaskExtremum, extremum)) {
					extremum = prefixPartOfMaskExtremum;
					extremumRow = row;
				}
			}
			else {
				if (this->compare(suffixPartOfMaskExtremum, extremum)) {
					extremum = suffixPartOfMaskExtremum;
					extremumRow = row;
				}
			}
		}
		newImage.at<uchar>(indices.row, affixMixMaskCenterIndex) = extremum;
		indices.lastExtremaRow[affixMixMaskCenterIndex] = extremumRow;
	}
}

void SmartImageFilter::setSuffixOnlyMaskExtremum( cv::Mat& newImage, Indices& indices, Precalculation& precalculation )
{
	auto extremum = precalculation[ getIndexSuffix( indices, indices.secondMask.left, indices.secondMask.right, indices.top ) ];
	auto extremumRow = indices.top;
	for (auto row = indices.top + 1; row <= indices.bot; ++row) {
		auto currentValue = precalculation[ getIndexSuffix( indices, indices.secondMask.left, indices.secondMask.right, row ) ];
		if (this->compare(currentValue, extremum)) {
			extremum = currentValue;
			extremumRow = row;
		}
	}
	newImage.at<uchar>(indices.row, indices.secondMask.center) = extremum;
	indices.lastExtremaRow[ indices.secondMask.center ] = extremumRow;
}

void SmartImageFilter::setExtrema( cv::Mat& newImage, Indices& indices, Precalculations& precalculations )
{
	//Finding best value for left-first index from first mask (it is separate case - we need to only use prefixes from first mask)
		//Iterating from top row of mask up to bottom row of mask
		setPrefixOnlyMaskExtremum( newImage, indices, precalculations.prefixes );
		//Finding best value for indices that are mix of prefixes from first mask and postfixes from second mask
		setAffixMixMaskExtrema( newImage, indices, precalculations );
		//Finding best value for last index from second mask (it is separate case - we need to only use postfixes from second mask)
		//Iterating from top row of mask up to bottom row of mask
		setSuffixOnlyMaskExtremum( newImage, indices, precalculations.suffixes );
}

int SmartImageFilter::getIndexPrefixFromRowToBeOverriden(const Indices& indices, int left, int right){
	auto relativeLeft = left;
	auto relativeRight = right;
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight + indices.rowToBeOverriden;
}

int SmartImageFilter::getIndexSuffixFromRowToBeOverriden(const Indices& indices, int left, int right){
	auto relativeLeft = left;
	auto relativeRight = right;
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight + indices.rowToBeOverriden;
}

int SmartImageFilter::getIndexPrefixFresh(const Indices& indices, int left, int right, int row){
	auto relativeLeft = left;
	auto relativeRight = right;
	auto relativeRow = row - indices.top;
	auto relativeRowWithOverridenOffset = relativeRow + indices.rowToBeOverriden + 1;
	if(relativeRowWithOverridenOffset >= this->maskSize){
		relativeRowWithOverridenOffset -= this->maskSize;
	}
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight + relativeRowWithOverridenOffset;
}

int SmartImageFilter::getIndexSuffixFresh(const Indices& indices, int left, int right, int row){
	auto relativeLeft = left;
	auto relativeRight = right;
	auto relativeRow = row - indices.top;
	auto relativeRowWithOverridenOffset = relativeRow + indices.rowToBeOverriden + 1;
	if(relativeRowWithOverridenOffset >= this->maskSize){
		relativeRowWithOverridenOffset -= this->maskSize;
	}
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight + relativeRowWithOverridenOffset;
}

void SmartImageFilter::setFreshPrefixOnlyMaskExtremum( cv::Mat& newImage, Indices& indices, Precalculation& precalculation )
{
	if( auto lastExtremumRow = indices.lastExtremaRow[indices.firstMask.center]; (indices.top <= lastExtremumRow) && (lastExtremumRow < indices.bot)){
		auto currentValue = precalculation[ getIndexPrefixFresh( indices, indices.firstMask.left, indices.firstMask.right, indices.bot ) ];
		auto previousValue = newImage.at<uchar>(indices.row - 1, indices.firstMask.center);
		if( this->compare(currentValue, previousValue) ){
			newImage.at<uchar>(indices.row, indices.firstMask.center) = currentValue;
			indices.lastExtremaRow[indices.firstMask.center] = indices.bot;
		} else{
			newImage.at<uchar>(indices.row, indices.firstMask.center) = previousValue;
		}
	} else{
		auto extremum = precalculation[ getIndexPrefixFresh(indices, indices.firstMask.left, indices.firstMask.right, indices.top) ];
		auto extremumRow = indices.top;
		for (auto row = indices.top + 1; row <= indices.bot; ++row) {
			auto currentValue = precalculation[ getIndexPrefixFresh( indices, indices.firstMask.left, indices.firstMask.right, row ) ];
			if (this->compare(currentValue, extremum)) {
				extremum = currentValue;
				extremumRow = row;
			}
		}
		newImage.at<uchar>(indices.row, indices.firstMask.center) = extremum;
		indices.lastExtremaRow[ indices.firstMask.center ] = extremumRow;
	}
}

void SmartImageFilter::setFreshAffixMixMaskExtrema( cv::Mat& newImage, Indices& indices, Precalculations& precalculations )
{
	auto maskOneHalfLength = static_cast<int>( std::floor(this->maskSize / 2) );
	// ten znak mniejsze rowne zamienic na mniejsze, bo jak rowne, to ten przypadek to second maska suffix po prostu
	for (auto affixMixMaskCenterIndex = indices.firstMask.center + 1; affixMixMaskCenterIndex < indices.secondMask.center; ++affixMixMaskCenterIndex) {
		auto currentMaskLeftIndex = std::clamp(affixMixMaskCenterIndex - maskOneHalfLength, indices.firstMask.left, indices.firstMask.right);
		auto currentMaskRightIndex = std::clamp(affixMixMaskCenterIndex + maskOneHalfLength, indices.secondMask.left, indices.secondMask.right);

		if( auto lastExtremumRow = indices.lastExtremaRow[affixMixMaskCenterIndex]; (indices.top <= lastExtremumRow) && (lastExtremumRow < indices.bot)){
			auto prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefixFresh( indices, currentMaskLeftIndex, indices.firstMask.right, indices.bot ) ];
			auto suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffixFresh( indices, indices.secondMask.left, currentMaskRightIndex, indices.bot ) ];

			auto currentValue = this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum) ? prefixPartOfMaskExtremum : suffixPartOfMaskExtremum ;
			auto previousValue = newImage.at<uchar>(indices.row - 1, affixMixMaskCenterIndex);
			if( this->compare(currentValue, previousValue) ){
				newImage.at<uchar>(indices.row, affixMixMaskCenterIndex) = currentValue;
				indices.lastExtremaRow[affixMixMaskCenterIndex] = indices.bot;
			} else{
				newImage.at<uchar>(indices.row, affixMixMaskCenterIndex) = previousValue;
			}
		}else{
			auto prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefixFresh( indices, currentMaskLeftIndex, indices.firstMask.right, indices.top ) ];
			auto suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffixFresh( indices, indices.secondMask.left, currentMaskRightIndex, indices.top ) ];

			auto extremum = this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum) ? prefixPartOfMaskExtremum : suffixPartOfMaskExtremum ;
			auto extremumRow = indices.top;
			//Iterating from top row of mask up to bottom row of mask
			for (auto row = indices.top + 1; row <= indices.bot; ++row) {
				prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefixFresh( indices, currentMaskLeftIndex, indices.firstMask.right, row ) ];
				suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffixFresh( indices, indices.secondMask.left, currentMaskRightIndex, row ) ];

				if (this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum)) {
					if (this->compare(prefixPartOfMaskExtremum, extremum)) {
						extremum = prefixPartOfMaskExtremum;
						extremumRow = row;
					}
				}
				else {
					if (this->compare(suffixPartOfMaskExtremum, extremum)) {
						extremum = suffixPartOfMaskExtremum;
						extremumRow = row;
					}
				}
			}
			newImage.at<uchar>(indices.row, affixMixMaskCenterIndex) = extremum;
			indices.lastExtremaRow[ affixMixMaskCenterIndex ] = extremumRow;
		}
	}
}

void SmartImageFilter::setFreshSuffixOnlyMaskExtremum( cv::Mat& newImage, Indices& indices, Precalculation& precalculation )
{
	if( auto lastExtremumRow = indices.lastExtremaRow[indices.secondMask.center]; (indices.top <= lastExtremumRow) && (lastExtremumRow < indices.bot)){
		auto currentValue = precalculation[ getIndexSuffixFresh( indices, indices.secondMask.left, indices.secondMask.right, indices.bot ) ];
		auto previousValue = newImage.at<uchar>(indices.row - 1, indices.secondMask.center);
		if( this->compare(currentValue, previousValue) ){
			newImage.at<uchar>(indices.row, indices.secondMask.center) = currentValue;
			indices.lastExtremaRow[indices.secondMask.center] = indices.bot;
		} else{
			newImage.at<uchar>(indices.row, indices.secondMask.center) = previousValue;
		}
	} else{
		auto extremum = precalculation[ getIndexSuffixFresh(indices, indices.secondMask.left, indices.secondMask.right, indices.top) ];
		auto extremumRow = indices.top;
		for (auto row = indices.top + 1; row <= indices.bot; ++row) {
			auto currentValue = precalculation[ getIndexSuffixFresh( indices, indices.secondMask.left, indices.secondMask.right, row ) ];
			if (this->compare(currentValue, extremum)) {
				extremum = currentValue;
				extremumRow = row;
			}
		}
		newImage.at<uchar>(indices.row, indices.secondMask.center) = extremum;
		indices.lastExtremaRow[ indices.secondMask.center ] = extremumRow;
	}
}

void SmartImageFilter::setFreshExtrema( cv::Mat& newImage, Indices& indices, Precalculations& precalculations )
{
	setFreshPrefixOnlyMaskExtremum( newImage, indices, precalculations.prefixes );
	setFreshAffixMixMaskExtrema( newImage, indices, precalculations );
	setFreshSuffixOnlyMaskExtremum( newImage, indices, precalculations.suffixes );
}

void SmartImageFilter::updateRowColumnAndIndex( int& row, int& column, int& index )
{
	auto valueToIterate = this->maskSize + 1;
	column += valueToIterate;  // Move to next mask position

	if (column >= this->data.cols) {  // Check if we need to transition to a new row
		++row;
		column = 0;
		index = row * this->data.cols;  // Move to the first column of the new row
	}
	else {
		index += valueToIterate;
	}
}

void SmartImageFilter::updateRowColumnAndIndices( int& row, int& column, int& index, Indices& indices )
{
	auto valueToIterate = this->maskSize + 1;
	column += valueToIterate;  // Move to next mask position

	if (column >= this->data.cols) {  // Check if we need to transition to a new row
		++row;
		if(++indices.rowToBeOverriden == this->maskSize){
			indices.rowToBeOverriden = 0;
		}
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
	affixesPrecalculations.prefixes.reserve( this->data.cols * this->data.cols * this->maskSize + this->data.cols * this->maskSize + this->maskSize );
	affixesPrecalculations.suffixes.reserve( this->data.cols * this->data.cols * this->maskSize + this->data.cols * this->maskSize + this->maskSize );

	auto initRow = firstIndex / this->data.cols;
	auto row = initRow;
	auto column = firstIndex - (row * this->data.cols);
	auto indices = Indices{};
	indices.lastExtremaRow.reserve(this->data.cols);
	indices.rowToBeOverriden = 0;
		
	// In this approach, I'm trying to mix the naive with smart approach,
	// smart approach is applied for each consecutive masks in columns - precalculating prefixes and suffixes,
	// but when we move to the next row, there might be a possiblity that we don't need to precalculate the whole height of a mask.
	// We might only need to check the newly added row. To decide, we need to keep the row index of last extremum
	// and check whether it is out of current mask. If so, we need to go through the precalculations, updated with a new row precalculation
	// and find the extremum. If not, we need to look up the value in the row above, not in the precalculations - in the image itself,
	// because it should store the extremum, and compare with the new precalculation
	//
	// There should be 3 different cases to consider, when iterating the image (part of image in case of multithreading):
	// 1. First row (or first two rows, if we dont start at column 0), where we do each precalculations
	//		- we do each precalculations, because it is our start for next rows and in case of not starting at column 0,
	//		  first column does not help in calculating middle rows, because it has masks at different indexes than these rows
	//		  starting at 0
	// 2. Middle rows, where we apply the naive algorithm explained in the description above
	// 3. End row, where we need to calculate last 2 masks differently, because once again, similarly to first row, we might end not at the last column
	// 
	// In 2. case, I will update the precalculations by just moving them one row up in the precalculations container and putting new row into the free space 
	
	// "i" is always the centre of first mask
	// 1. case
	auto i = firstIndex;
	auto breakInitRows = row+this->maskSize;
	for (; i <= lastIndex && row != breakInitRows; ) {
		updateIndices(indices, row, column);

		precalculate( indices, affixesPrecalculations );
		setExtrema( newImage, indices, affixesPrecalculations );

		updateRowColumnAndIndex( row, column, i);
	}

	// 2. case
	for (; i <= lastIndex; ) {
		updateIndices(indices, row, column);

		precalculateFreshRow( indices, affixesPrecalculations );
		setFreshExtrema( newImage, indices, affixesPrecalculations );

		updateRowColumnAndIndices( row, column, i, indices);
	}

	for (; i <= lastIndex; ) {
		updateIndices(indices, row, column);

		precalculate( indices, affixesPrecalculations );
		setExtrema( newImage, indices, affixesPrecalculations );

		updateRowColumnAndIndex(row, column, i);
	}
}