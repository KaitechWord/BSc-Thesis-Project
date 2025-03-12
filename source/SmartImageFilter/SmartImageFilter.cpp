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

////////////////////////////////// COLUMN APPROACH

int SmartImageFilter::getIndexPrefixColumn( const Indices& indices, int left, int right, int row ){
	auto relativeLeft = left;
	auto relativeRight = right;
	// auto relativeRow = row - indices.top;
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight /*+ relativeRow*/;
}

int SmartImageFilter::getIndexSuffixColumn( const Indices& indices, int left, int right, int row ){
	auto relativeLeft = left;
	auto relativeRight = right;
	// auto relativeRow = row - indices.top;
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight /*+ relativeRow*/;
}

void SmartImageFilter::calculatePrefixesColumn( const Indices& indices, Precalculation& precalculation, int row )
{
	auto prefixRightIndex = indices.firstMask.right;
	precalculation[ getIndexPrefixColumn( indices, prefixRightIndex, prefixRightIndex, row ) ] = this->data.at<uchar>(row, prefixRightIndex);
	for (auto prefixLeftIndex = prefixRightIndex - 1; prefixLeftIndex >= indices.firstMask.left; --prefixLeftIndex) {
		auto freshValue = this->data.at<uchar>(row, prefixLeftIndex);
		auto previousPrefixValue = precalculation[ getIndexPrefixColumn( indices, (prefixLeftIndex + 1), prefixRightIndex, row ) ];
		precalculation[ getIndexPrefixColumn( indices, prefixLeftIndex, prefixRightIndex, row ) ] = this->compare(freshValue, previousPrefixValue) ? freshValue : previousPrefixValue;
	}
}

void SmartImageFilter::calculateSuffixesColumn( const Indices& indices, Precalculation& precalculation, int row )
{
	auto suffixLeftIndex = indices.secondMask.left; 
	precalculation[ getIndexSuffixColumn( indices, suffixLeftIndex, suffixLeftIndex, row ) ] = this->data.at<uchar>(row, suffixLeftIndex);
	for (auto suffixRightIndex = suffixLeftIndex + 1; suffixRightIndex <= indices.secondMask.right; ++suffixRightIndex) {
		auto freshValue = this->data.at<uchar>(row, suffixRightIndex);
		auto previousSuffixValue = precalculation[ getIndexSuffixColumn( indices, suffixLeftIndex, suffixRightIndex - 1, row ) ];
		precalculation[ getIndexSuffixColumn( indices, suffixLeftIndex, suffixRightIndex, row ) ] = this->compare(freshValue, previousSuffixValue) ? freshValue : previousSuffixValue;
	}
}

void SmartImageFilter::precalculateColumn( const Indices& indices, Precalculations& precalculations )
{
	calculatePrefixesColumn( indices, precalculations.prefixes, indices.row );
	calculateSuffixesColumn( indices, precalculations.suffixes, indices.row );
}

void SmartImageFilter::setPrefixOnlyMaskExtremumColumn( cv::Mat& newImage, Indices& indices, Precalculation& precalculation )
{
	// auto extremum = precalculation[ getIndexPrefixColumn(indices, indices.firstMask.left, indices.firstMask.right, indices.row) ];
	// auto extremumRow = indices.top;
	// for (auto row = indices.top + 1; row <= indices.bot; ++row) {
		// auto currentValue = precalculation[ getIndexPrefixColumn( indices, indices.firstMask.left, indices.firstMask.right, row ) ];
		// if (this->compare(currentValue, extremum)) {
			// extremum = currentValue;
			// extremumRow = row;
		// }
	// }
	// newImage.at<uchar>(indices.row, indices.firstMask.center) = extremum;
	// indices.lastExtremaRow[ indices.firstMask.center ] = extremumRow;
	newImage.at<uchar>(indices.row, indices.firstMask.center) = precalculation[ getIndexPrefixColumn(indices, indices.firstMask.left, indices.firstMask.right, indices.row) ];
}

void SmartImageFilter::setAffixMixMaskExtremaColumn( cv::Mat& newImage, Indices& indices, Precalculations& precalculations )
{
	auto maskOneHalfLength = static_cast<int>( std::floor(this->maskSize / 2) );
	for (auto affixMixMaskCenterIndex = indices.firstMask.center + 1; affixMixMaskCenterIndex < indices.secondMask.center; ++affixMixMaskCenterIndex) {
		
		auto currentMaskLeftIndex = std::clamp(affixMixMaskCenterIndex - maskOneHalfLength, indices.firstMask.left, indices.firstMask.right);
		auto currentMaskRightIndex = std::clamp(affixMixMaskCenterIndex + maskOneHalfLength, indices.secondMask.left, indices.secondMask.right);

		auto prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefixColumn( indices, currentMaskLeftIndex, indices.firstMask.right, indices.row ) ];
		auto suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffixColumn( indices, indices.secondMask.left, currentMaskRightIndex, indices.row ) ];

		auto extremum = this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum) ? prefixPartOfMaskExtremum : suffixPartOfMaskExtremum ;
		// auto  extremumRow= indices.top;
		
		//Iterating from top row of mask up to bottom row of mask
		// for (auto row = indices.top + 1; row <= indices.bot; ++row) {
			// prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefixColumn( indices, currentMaskLeftIndex, indices.firstMask.right, row ) ];
			// suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffixColumn( indices, indices.secondMask.left, currentMaskRightIndex, row ) ];

			// if (this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum)) {
				// if (this->compare(prefixPartOfMaskExtremum, extremum)) {
					// extremum = prefixPartOfMaskExtremum;
					// extremumRow = row;
				// }
			// }
			// else {
				// if (this->compare(suffixPartOfMaskExtremum, extremum)) {
					// extremum = suffixPartOfMaskExtremum;
					// extremumRow = row;
				// }
			// }
		// }
		newImage.at<uchar>(indices.row, affixMixMaskCenterIndex) = extremum;
		// indices.lastExtremaRow[affixMixMaskCenterIndex] = extremumRow;
	}
}

void SmartImageFilter::setSuffixOnlyMaskExtremumColumn( cv::Mat& newImage, Indices& indices, Precalculation& precalculation )
{
	// auto extremum = precalculation[ getIndexSuffixColumn( indices, indices.secondMask.left, indices.secondMask.right, indices.top ) ];
	// auto extremumRow = indices.top;
	// for (auto row = indices.top + 1; row <= indices.bot; ++row) {
		// auto currentValue = precalculation[ getIndexSuffixColumn( indices, indices.secondMask.left, indices.secondMask.right, row ) ];
		// if (this->compare(currentValue, extremum)) {
			// extremum = currentValue;
			// extremumRow = row;
		// }
	// }
	// newImage.at<uchar>(indices.row, indices.secondMask.center) = extremum;
	// indices.lastExtremaRow[ indices.secondMask.center ] = extremumRow;
	newImage.at<uchar>(indices.row, indices.secondMask.center) = precalculation[ getIndexSuffixColumn( indices, indices.secondMask.left, indices.secondMask.right, indices.row ) ];
}

void SmartImageFilter::setExtremaColumn( cv::Mat& newImage, Indices& indices, Precalculations& precalculations )
{
	//Finding best value for left-first index from first mask (it is separate case - we need to only use prefixes from first mask)
		//Iterating from top row of mask up to bottom row of mask
		setPrefixOnlyMaskExtremumColumn( newImage, indices, precalculations.prefixes );
		//Finding best value for indices that are mix of prefixes from first mask and postfixes from second mask
		setAffixMixMaskExtremaColumn( newImage, indices, precalculations );
		//Finding best value for last index from second mask (it is separate case - we need to only use postfixes from second mask)
		//Iterating from top row of mask up to bottom row of mask
		setSuffixOnlyMaskExtremumColumn( newImage, indices, precalculations.suffixes );
}

void SmartImageFilter::updateRowColumnAndIndexColumn( int& row, int& column, int& index )
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

void SmartImageFilter::updateRowColumnAndIndicesColumn( int& row, int& column, int& index, Indices& indices )
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

void SmartImageFilter::updateIndicesColumn( Indices& indices, int row, int column ){
	auto maskOneHalfLength = static_cast<int>(std::floor(this->maskSize / 2));
	indices.firstMask.center = column;
	indices.firstMask.left = std::max(indices.firstMask.center - maskOneHalfLength, 0);
	indices.firstMask.right = std::min(indices.firstMask.center + maskOneHalfLength, this->data.cols - 1);

	indices.secondMask.center = std::clamp(indices.firstMask.center + this->maskSize, 0, this->data.cols - 1);;
	indices.secondMask.left = std::max(indices.secondMask.center - maskOneHalfLength, 0);
	indices.secondMask.right = std::min(indices.secondMask.center + maskOneHalfLength, this->data.cols - 1);

	indices.row = row;
}

////////////////////////////////// ROW APPROACH

void SmartImageFilter::updateIndicesRow( Indices& indices, int row, int column ){
	auto maskOneHalfLength = static_cast<int>(std::floor(this->maskSize / 2));
	indices.firstMask.center = row;
	indices.firstMask.left = std::max(indices.firstMask.center - maskOneHalfLength, 0);
	indices.firstMask.right = std::min(indices.firstMask.center + maskOneHalfLength, this->data.rows - 1);

	indices.secondMask.center = std::clamp(indices.firstMask.center + this->maskSize, 0, this->data.rows - 1);;
	indices.secondMask.left = std::max(indices.secondMask.center - maskOneHalfLength, 0);
	indices.secondMask.right = std::min(indices.secondMask.center + maskOneHalfLength, this->data.rows - 1);

	indices.col = column;
}

int SmartImageFilter::getIndexPrefixRow( const Indices& indices, int left, int right, int col ){
	auto relativeLeft = left;
	auto relativeRight = right;
	// auto relativeCol = col - indices.top;
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight /*+ relativeRow*/;
}

int SmartImageFilter::getIndexSuffixRow( const Indices& indices, int left, int right, int col ){
	auto relativeLeft = left;
	auto relativeRight = right;
	// auto relativeRow = row - indices.top;
	return this->maskSize * this->maskSize * relativeLeft + this->maskSize * relativeRight /*+ relativeRow*/;
}

void SmartImageFilter::calculatePrefixesRow( cv::Mat& indirect, const Indices& indices, Precalculation& precalculation, int col )
{
	auto prefixRightIndex = indices.firstMask.right;
	precalculation[ getIndexPrefixRow( indices, prefixRightIndex, prefixRightIndex, col ) ] = indirect.at<uchar>(prefixRightIndex, col);
	for (auto prefixLeftIndex = prefixRightIndex - 1; prefixLeftIndex >= indices.firstMask.left; --prefixLeftIndex) {
		auto freshValue = indirect.at<uchar>(prefixLeftIndex, col);
		auto previousPrefixValue = precalculation[ getIndexPrefixRow( indices, (prefixLeftIndex + 1), prefixRightIndex, col ) ];
		precalculation[ getIndexPrefixRow( indices, prefixLeftIndex, prefixRightIndex, col ) ] = this->compare(freshValue, previousPrefixValue) ? freshValue : previousPrefixValue;
	}
}

void SmartImageFilter::calculateSuffixesRow( cv::Mat& indirect, const Indices& indices, Precalculation& precalculation, int col )
{
	auto suffixLeftIndex = indices.secondMask.left; 
	precalculation[ getIndexSuffixRow( indices, suffixLeftIndex, suffixLeftIndex, col ) ] = indirect.at<uchar>(suffixLeftIndex, col);
	for (auto suffixRightIndex = suffixLeftIndex + 1; suffixRightIndex <= indices.secondMask.right; ++suffixRightIndex) {
		auto freshValue = indirect.at<uchar>(suffixRightIndex, col);
		auto previousSuffixValue = precalculation[ getIndexSuffixRow( indices, suffixLeftIndex, suffixRightIndex - 1, col ) ];
		precalculation[ getIndexSuffixRow( indices, suffixLeftIndex, suffixRightIndex, col ) ] = this->compare(freshValue, previousSuffixValue) ? freshValue : previousSuffixValue;
	}
}

void SmartImageFilter::setPrefixOnlyMaskExtremumRow( cv::Mat& newImage, Indices& indices, Precalculation& precalculation )
{
	// auto extremum = precalculation[ getIndexPrefixColumn(indices, indices.firstMask.left, indices.firstMask.right, indices.row) ];
	// auto extremumRow = indices.top;
	// for (auto row = indices.top + 1; row <= indices.bot; ++row) {
		// auto currentValue = precalculation[ getIndexPrefixColumn( indices, indices.firstMask.left, indices.firstMask.right, row ) ];
		// if (this->compare(currentValue, extremum)) {
			// extremum = currentValue;
			// extremumRow = row;
		// }
	// }
	// newImage.at<uchar>(indices.row, indices.firstMask.center) = extremum;
	// indices.lastExtremaRow[ indices.firstMask.center ] = extremumRow;
	newImage.at<uchar>(indices.firstMask.center, indices.col) = precalculation[ getIndexPrefixRow(indices, indices.firstMask.left, indices.firstMask.right, indices.col) ];
}

void SmartImageFilter::setAffixMixMaskExtremaRow( cv::Mat& newImage, Indices& indices, Precalculations& precalculations )
{
	auto maskOneHalfLength = static_cast<int>( std::floor(this->maskSize / 2) );
	for (auto affixMixMaskCenterIndex = indices.firstMask.center + 1; affixMixMaskCenterIndex < indices.secondMask.center; ++affixMixMaskCenterIndex) {
		
		auto currentMaskLeftIndex = std::clamp(affixMixMaskCenterIndex - maskOneHalfLength, indices.firstMask.left, indices.firstMask.right);
		auto currentMaskRightIndex = std::clamp(affixMixMaskCenterIndex + maskOneHalfLength, indices.secondMask.left, indices.secondMask.right);

		auto prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefixRow( indices, currentMaskLeftIndex, indices.firstMask.right, indices.col ) ];
		auto suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffixRow( indices, indices.secondMask.left, currentMaskRightIndex, indices.col ) ];

		auto extremum = this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum) ? prefixPartOfMaskExtremum : suffixPartOfMaskExtremum ;
		// auto  extremumRow= indices.top;
		
		//Iterating from top row of mask up to bottom row of mask
		// for (auto row = indices.top + 1; row <= indices.bot; ++row) {
			// prefixPartOfMaskExtremum = precalculations.prefixes[ getIndexPrefixColumn( indices, currentMaskLeftIndex, indices.firstMask.right, row ) ];
			// suffixPartOfMaskExtremum = precalculations.suffixes[ getIndexSuffixColumn( indices, indices.secondMask.left, currentMaskRightIndex, row ) ];

			// if (this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum)) {
				// if (this->compare(prefixPartOfMaskExtremum, extremum)) {
					// extremum = prefixPartOfMaskExtremum;
					// extremumRow = row;
				// }
			// }
			// else {
				// if (this->compare(suffixPartOfMaskExtremum, extremum)) {
					// extremum = suffixPartOfMaskExtremum;
					// extremumRow = row;
				// }
			// }
		// }
		newImage.at<uchar>(affixMixMaskCenterIndex, indices.col) = extremum;
		// indices.lastExtremaRow[affixMixMaskCenterIndex] = extremumRow;
	}
}

void SmartImageFilter::setSuffixOnlyMaskExtremumRow( cv::Mat& newImage, Indices& indices, Precalculation& precalculation )
{
	// auto extremum = precalculation[ getIndexSuffixColumn( indices, indices.secondMask.left, indices.secondMask.right, indices.top ) ];
	// auto extremumRow = indices.top;
	// for (auto row = indices.top + 1; row <= indices.bot; ++row) {
		// auto currentValue = precalculation[ getIndexSuffixColumn( indices, indices.secondMask.left, indices.secondMask.right, row ) ];
		// if (this->compare(currentValue, extremum)) {
			// extremum = currentValue;
			// extremumRow = row;
		// }
	// }
	// newImage.at<uchar>(indices.row, indices.secondMask.center) = extremum;
	// indices.lastExtremaRow[ indices.secondMask.center ] = extremumRow;
	newImage.at<uchar>(indices.secondMask.center, indices.col) = precalculation[ getIndexSuffixRow( indices, indices.secondMask.left, indices.secondMask.right, indices.col ) ];
}

void SmartImageFilter::setExtremaRow( cv::Mat& newImage, Indices& indices, Precalculations& precalculations )
{
	//Finding best value for left-first index from first mask (it is separate case - we need to only use prefixes from first mask)
		//Iterating from top row of mask up to bottom row of mask
		setPrefixOnlyMaskExtremumRow( newImage, indices, precalculations.prefixes );
		//Finding best value for indices that are mix of prefixes from first mask and postfixes from second mask
		setAffixMixMaskExtremaRow( newImage, indices, precalculations );
		//Finding best value for last index from second mask (it is separate case - we need to only use postfixes from second mask)
		//Iterating from top row of mask up to bottom row of mask
		setSuffixOnlyMaskExtremumRow( newImage, indices, precalculations.suffixes );
}

bool SmartImageFilter::updateRowColumnRow( int& row, int& column, int lastRow, int lastCol )
{
	// row += this->maskSize + 1;

	// if( column == lastCol &&  )

	// if (row >= lastRow || ( column == lastCol && row >= ( lastRow - 1 ) ) ) {
	// 	++column;
	// 	if(column >= this->data.cols)
	// 		column = 0;
	// 	row = 0;
	// }
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

	// IMPROVE THAT SIZE
	affixesPrecalculations.prefixes.reserve( this->data.cols * this->data.cols * this->maskSize + this->data.cols * this->maskSize + this->maskSize );
	affixesPrecalculations.suffixes.reserve( this->data.cols * this->data.cols * this->maskSize + this->data.cols * this->maskSize + this->maskSize );

	auto initRow = firstIndex / this->data.cols;
	auto initCol = firstIndex - (initRow * this->data.cols);
	auto lastRow = lastIndex / this->data.cols;
	auto lastCol = lastIndex - (lastRow * this->data.cols);
	auto row = initRow;
	auto column = initCol;
	auto indices = Indices{};
	cv::Mat indirect{ newImage.size(), newImage.type() };

	auto maskOneHalfLength = static_cast<int>( std::floor(this->maskSize / 2) );
	auto initRowColumn = std::max( initRow - maskOneHalfLength, 0 );
	auto initColColumn = std::max( initCol - maskOneHalfLength, 0 );
	auto lastRowColumn = std::min( lastRow + maskOneHalfLength, this->data.rows - 1 );
	auto lastColColumn = std::min( lastCol + maskOneHalfLength, this->data.cols - 1 );
	auto firstIndexColumn = initRowColumn * this->data.cols + initColColumn;
	auto lastIndexColumn = lastRowColumn * this->data.cols + lastColColumn;

	auto i = firstIndexColumn;
	for (; i <= lastIndexColumn; ) {
		updateIndicesColumn(indices, initRowColumn, initColColumn);

		precalculateColumn( indices, affixesPrecalculations );
		setExtremaColumn( indirect, indices, affixesPrecalculations );

		updateRowColumnAndIndexColumn( initRowColumn, initColColumn, i);
	}
	row = initRow;
	column = this->data.cols - 1;
	affixesPrecalculations.prefixes.clear();
	affixesPrecalculations.suffixes.clear();
	
	for (; column != -1; ) {
		updateIndicesRow(indices, row, column);

		calculatePrefixesRow( indirect, indices, affixesPrecalculations.prefixes, indices.col );
		calculateSuffixesRow( indirect, indices, affixesPrecalculations.suffixes, indices.col );
		setExtremaRow( newImage, indices, affixesPrecalculations );
		
		{
			row += this->maskSize + 1;

			if (row >= lastRow || ( column > lastCol && row >= ( lastRow - 1 ) ) ) {
				if( --column < initCol  )
					row = initRow + 1;
				else
					row = initRow;
			}
		}
	}
}