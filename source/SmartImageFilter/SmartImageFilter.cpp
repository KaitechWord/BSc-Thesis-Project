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
{
}

void SmartImageFilter::apply(cv::Mat &image)
{
	this->data = image;
	auto rows = image.rows;
	auto cols = image.cols;
	auto pixelsNum = rows * cols;
	auto threadsNum = this->threadNum;
	if (threadsNum > pixelsNum)
	{
		std::cout << "The number of threads is bigger than the size of the image. Setting number of threads to size.\n";
		threadsNum = pixelsNum;
	}
	auto sizeOfOneThread = pixelsNum / threadsNum;
	auto remainder = pixelsNum % threadsNum;

	std::vector<std::thread> threads;
	cv::Mat newImage(rows, cols, image.type());
	auto start = std::chrono::high_resolution_clock::now();
	for (auto i = 0; i < threadsNum; ++i)
	{
		auto firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		auto lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		threads.emplace_back(std::thread(&SmartImageFilter::filter, this, std::ref(newImage), firstIndex, lastIndex));
	}
	for (auto &thread : threads)
		thread.join();
	auto end = std::chrono::high_resolution_clock::now();
	const auto execTime = std::chrono::duration<double, std::milli>(end - start).count();
	std::cout << std::fixed;
	std::cout << std::setprecision(2);
	std::cout << "Smart " << (this->algType == AlgorithmType::MIN ? "min." : "max.") << " image " << (threadsNum == 1 ? "(single-threaded)" : "(multi-threaded)") << " MaskSize: " << this->maskSize << " filter execution time : " << execTime << "ms.\n Elem./sec.: " << (this->data.cols * this->data.rows / std::chrono::duration<double, std::milli>(end - start).count()) * 0.001 << "\n";
	const auto textFile = this->algType == AlgorithmType::MIN ? (threadsNum == 1 ? minSingleTextFile : minMultiTextFile) : (threadsNum == 1 ? maxSingleTextFile : maxMultiTextFile);
	std::ofstream outfile;
	outfile.open(textFile, std::ios_base::app);
	if (outfile.is_open())
		outfile << execTime << "\n";
	else
		std::cerr << "File: " << textFile << " did not open successfully." << "\n";
	for (auto i = 0; i < threadsNum; ++i)
	{
		auto firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		auto lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		auto partOfimageIndex = 0;
		auto colSize = this->data.cols;
		for (auto j = firstIndex; j <= lastIndex; ++j)
		{
			image.at<uchar>(j / colSize, j % colSize) = newImage.at<uchar>(j / colSize, j % colSize);
		}
	}
	outfile.close();
}

////////////////////////////////// COLUMN APPROACH

int SmartImageFilter::getIndex(int left, int right)
{
	return this->maskSize * left + right;
}

void SmartImageFilter::calculatePrefixesColumn(const Indices &indices, Precalculation &precalculation, int row)
{
	auto prefixRightIndex = indices.firstMask.right;
	precalculation[getIndex(prefixRightIndex, prefixRightIndex)] = this->data.at<uchar>(row, prefixRightIndex);
	for (auto prefixLeftIndex = prefixRightIndex - 1; prefixLeftIndex >= indices.firstMask.left; --prefixLeftIndex)
	{
		auto freshValue = this->data.at<uchar>(row, prefixLeftIndex);
		auto previousPrefixValue = precalculation[getIndex((prefixLeftIndex + 1), prefixRightIndex)];
		precalculation[getIndex(prefixLeftIndex, prefixRightIndex)] = this->compare(freshValue, previousPrefixValue) ? freshValue : previousPrefixValue;
	}
}

void SmartImageFilter::calculateSuffixesColumn(const Indices &indices, Precalculation &precalculation, int row)
{
	auto suffixLeftIndex = indices.secondMask.left;
	precalculation[getIndex(suffixLeftIndex, suffixLeftIndex)] = this->data.at<uchar>(row, suffixLeftIndex);
	for (auto suffixRightIndex = suffixLeftIndex + 1; suffixRightIndex <= indices.secondMask.right; ++suffixRightIndex)
	{
		auto freshValue = this->data.at<uchar>(row, suffixRightIndex);
		auto previousSuffixValue = precalculation[getIndex(suffixLeftIndex, suffixRightIndex - 1)];
		precalculation[getIndex(suffixLeftIndex, suffixRightIndex)] = this->compare(freshValue, previousSuffixValue) ? freshValue : previousSuffixValue;
	}
}

void SmartImageFilter::precalculateColumn(const Indices &indices, Precalculations &precalculations)
{
	calculatePrefixesColumn(indices, precalculations.prefixes, indices.row);
	calculateSuffixesColumn(indices, precalculations.suffixes, indices.row);
}

void SmartImageFilter::setPrefixOnlyMaskExtremumColumn(cv::Mat &newImage, Indices &indices, Precalculation &precalculation)
{
	newImage.at<uchar>(indices.row, indices.firstMask.center) = precalculation[getIndex(indices.firstMask.left, indices.firstMask.right)];
}

void SmartImageFilter::setAffixMixMaskExtremaColumn(cv::Mat &newImage, Indices &indices, Precalculations &precalculations)
{
	auto maskOneHalfLength = static_cast<int>(std::floor(this->maskSize / 2));
	for (auto affixMixMaskCenterIndex = indices.firstMask.center + 1; affixMixMaskCenterIndex < indices.secondMask.center; ++affixMixMaskCenterIndex)
	{

		auto currentMaskLeftIndex = std::clamp(affixMixMaskCenterIndex - maskOneHalfLength, indices.firstMask.left, indices.firstMask.right);
		auto currentMaskRightIndex = std::clamp(affixMixMaskCenterIndex + maskOneHalfLength, indices.secondMask.left, indices.secondMask.right);

		auto prefixPartOfMaskExtremum = precalculations.prefixes[getIndex(currentMaskLeftIndex, indices.firstMask.right)];
		auto suffixPartOfMaskExtremum = precalculations.suffixes[getIndex(indices.secondMask.left, currentMaskRightIndex)];

		auto extremum = this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum) ? prefixPartOfMaskExtremum : suffixPartOfMaskExtremum;

		newImage.at<uchar>(indices.row, affixMixMaskCenterIndex) = extremum;
	}
}

void SmartImageFilter::setSuffixOnlyMaskExtremumColumn(cv::Mat &newImage, Indices &indices, Precalculation &precalculation)
{
	newImage.at<uchar>(indices.row, indices.secondMask.center) = precalculation[getIndex(indices.secondMask.left, indices.secondMask.right)];
}

void SmartImageFilter::setExtremaColumn(cv::Mat &newImage, Indices &indices, Precalculations &precalculations)
{
	setPrefixOnlyMaskExtremumColumn(newImage, indices, precalculations.prefixes);
	setAffixMixMaskExtremaColumn(newImage, indices, precalculations);
	setSuffixOnlyMaskExtremumColumn(newImage, indices, precalculations.suffixes);
}

void SmartImageFilter::updateRowColumnAndIndex(int &row, int &column, int &index)
{
	auto valueToIterate = this->maskSize + 1;
	column += valueToIterate;

	if (column >= this->data.cols)
	{
		++row;
		column = 0;
		index = row * this->data.cols;
	}
	else
	{
		index += valueToIterate;
	}
}

void SmartImageFilter::updateIndicesColumn(Indices &indices, int row, int column)
{
	auto maskOneHalfLength = static_cast<int>(std::floor(this->maskSize / 2));
	indices.firstMask.center = column;
	indices.firstMask.left = std::max(indices.firstMask.center - maskOneHalfLength, 0);
	indices.firstMask.right = std::min(indices.firstMask.center + maskOneHalfLength, this->data.cols - 1);

	indices.secondMask.center = std::clamp(indices.firstMask.center + this->maskSize, 0, this->data.cols - 1);
	;
	indices.secondMask.left = std::max(indices.secondMask.center - maskOneHalfLength, 0);
	indices.secondMask.right = std::min(indices.secondMask.center + maskOneHalfLength, this->data.cols - 1);

	indices.row = row;
}

////////////////////////////////// ROW APPROACH

void SmartImageFilter::updateIndicesRow(Indices &indices, int row, int column)
{
	auto maskOneHalfLength = static_cast<int>(std::floor(this->maskSize / 2));
	indices.firstMask.center = row;
	indices.firstMask.left = std::max(indices.firstMask.center - maskOneHalfLength, 0);
	indices.firstMask.right = std::min(indices.firstMask.center + maskOneHalfLength, this->data.rows - 1);

	indices.secondMask.center = std::clamp(indices.firstMask.center + this->maskSize, 0, this->data.rows - 1);
	;
	indices.secondMask.left = std::max(indices.secondMask.center - maskOneHalfLength, 0);
	indices.secondMask.right = std::min(indices.secondMask.center + maskOneHalfLength, this->data.rows - 1);

	indices.col = column;
}

void SmartImageFilter::calculatePrefixesRow(cv::Mat &indirect, const Indices &indices, Precalculation &precalculation, int col)
{
	auto prefixRightIndex = indices.firstMask.right;
	precalculation[getIndex(prefixRightIndex, prefixRightIndex)] = indirect.at<uchar>(prefixRightIndex, col);
	for (auto prefixLeftIndex = prefixRightIndex - 1; prefixLeftIndex >= indices.firstMask.left; --prefixLeftIndex)
	{
		auto freshValue = indirect.at<uchar>(prefixLeftIndex, col);
		auto previousPrefixValue = precalculation[getIndex((prefixLeftIndex + 1), prefixRightIndex)];
		precalculation[getIndex(prefixLeftIndex, prefixRightIndex)] = this->compare(freshValue, previousPrefixValue) ? freshValue : previousPrefixValue;
	}
}

void SmartImageFilter::calculateSuffixesRow(cv::Mat &indirect, const Indices &indices, Precalculation &precalculation, int col)
{
	auto suffixLeftIndex = indices.secondMask.left;
	precalculation[getIndex(suffixLeftIndex, suffixLeftIndex)] = indirect.at<uchar>(suffixLeftIndex, col);
	for (auto suffixRightIndex = suffixLeftIndex + 1; suffixRightIndex <= indices.secondMask.right; ++suffixRightIndex)
	{
		auto freshValue = indirect.at<uchar>(suffixRightIndex, col);
		auto previousSuffixValue = precalculation[getIndex(suffixLeftIndex, suffixRightIndex - 1)];
		precalculation[getIndex(suffixLeftIndex, suffixRightIndex)] = this->compare(freshValue, previousSuffixValue) ? freshValue : previousSuffixValue;
	}
}

void SmartImageFilter::setPrefixOnlyMaskExtremumRow(cv::Mat &newImage, Indices &indices, Precalculation &precalculation)
{
	newImage.at<uchar>(indices.firstMask.center, indices.col) = precalculation[getIndex(indices.firstMask.left, indices.firstMask.right)];
}

void SmartImageFilter::setAffixMixMaskExtremaRow(cv::Mat &newImage, Indices &indices, Precalculations &precalculations)
{
	auto maskOneHalfLength = static_cast<int>(std::floor(this->maskSize / 2));
	for (auto affixMixMaskCenterIndex = indices.firstMask.center + 1; affixMixMaskCenterIndex < indices.secondMask.center; ++affixMixMaskCenterIndex)
	{

		auto currentMaskLeftIndex = std::clamp(affixMixMaskCenterIndex - maskOneHalfLength, indices.firstMask.left, indices.firstMask.right);
		auto currentMaskRightIndex = std::clamp(affixMixMaskCenterIndex + maskOneHalfLength, indices.secondMask.left, indices.secondMask.right);

		auto prefixPartOfMaskExtremum = precalculations.prefixes[getIndex(currentMaskLeftIndex, indices.firstMask.right)];
		auto suffixPartOfMaskExtremum = precalculations.suffixes[getIndex(indices.secondMask.left, currentMaskRightIndex)];

		auto extremum = this->compare(prefixPartOfMaskExtremum, suffixPartOfMaskExtremum) ? prefixPartOfMaskExtremum : suffixPartOfMaskExtremum;

		newImage.at<uchar>(affixMixMaskCenterIndex, indices.col) = extremum;
	}
}

void SmartImageFilter::setSuffixOnlyMaskExtremumRow(cv::Mat &newImage, Indices &indices, Precalculation &precalculation)
{
	newImage.at<uchar>(indices.secondMask.center, indices.col) = precalculation[getIndex(indices.secondMask.left, indices.secondMask.right)];
}

void SmartImageFilter::setExtremaRow(cv::Mat &newImage, Indices &indices, Precalculations &precalculations)
{
	setPrefixOnlyMaskExtremumRow(newImage, indices, precalculations.prefixes);
	setAffixMixMaskExtremaRow(newImage, indices, precalculations);
	setSuffixOnlyMaskExtremumRow(newImage, indices, precalculations.suffixes);
}

void SmartImageFilter::updateRowColumn(int &row, int &column, int initRow, int initCol, int lastRow, int lastCol)
{
	row += this->maskSize + 1;

	if (row > lastRow || column > lastCol && row > (lastRow - 1))
	{
		--column;
		if (column < initCol)
			row = initRow + 1;
		else
			row = initRow;
	}
}

void SmartImageFilter::filter(cv::Mat &newImage, int firstIndex, int lastIndex)
{
	// By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	//  auto maskOneHalfLength = this->maskSize / 2;
	// Starting value is set in base class in regards to algType
	//  auto targetValue = this->startingValue;

	// First [) is first index of prefix/suffix, second [) is last index of prefix/suffix, [) is row
	// An affix is a general term for a linguistic element added to a word, which includes:
	//  Prefix (before the root)
	//  Suffix (after the root)
	//  Infix (inside the root, though rare in English)
	Precalculations affixesPrecalculations;

	affixesPrecalculations.prefixes.reserve(this->data.rows * this->maskSize + this->data.rows);
	affixesPrecalculations.suffixes.reserve(this->data.rows * this->maskSize + this->data.rows);

	auto initRow = firstIndex / this->data.cols;
	auto initCol = firstIndex - (initRow * this->data.cols);
	auto lastRow = lastIndex / this->data.cols;
	auto lastCol = lastIndex - (lastRow * this->data.cols);
	auto row = initRow;
	auto column = initCol;
	auto indices = Indices{};
	cv::Mat indirect{newImage.size(), newImage.type()};

	auto padding = 2 * this->maskSize;
	auto initRowColumn = std::max(initRow - padding, 0);
	auto initColColumn = std::max(initCol - padding, 0);
	auto lastRowColumn = std::min(lastRow + padding, this->data.rows - 1);
	auto lastColColumn = std::min(lastCol + padding, this->data.cols - 1);
	auto firstIndexColumn = initRowColumn * this->data.cols + initColColumn;
	auto lastIndexColumn = lastRowColumn * this->data.cols + lastColColumn;

	auto i = firstIndexColumn;
	for (; i <= lastIndexColumn;)
	{
		updateIndicesColumn(indices, initRowColumn, initColColumn);

		precalculateColumn(indices, affixesPrecalculations);
		setExtremaColumn(indirect, indices, affixesPrecalculations);

		updateRowColumnAndIndex(initRowColumn, initColColumn, i);
	}
	row = initRow;
	column = this->data.cols - 1;
	affixesPrecalculations.prefixes.clear();
	affixesPrecalculations.suffixes.clear();

	for (; column != -1;)
	{
		updateIndicesRow(indices, row, column);

		calculatePrefixesRow(indirect, indices, affixesPrecalculations.prefixes, indices.col);
		calculateSuffixesRow(indirect, indices, affixesPrecalculations.suffixes, indices.col);
		setExtremaRow(newImage, indices, affixesPrecalculations);

		updateRowColumn(row, column, initRow, initCol, lastRow, lastCol);
	}
}