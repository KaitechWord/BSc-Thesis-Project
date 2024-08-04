#include "SmartImageFilter.h"
#include <iostream>
#include <chrono>
#include <map>
#include <optional>

SmartImageFilter::SmartImageFilter(int threadNum, AlgorithmType algType, int maskSize)
	: ImageFilter(threadNum, algType, maskSize)
{}

void SmartImageFilter::apply(cv::Mat& image) {
	this->data = image;
	int rows = image.rows;
	int cols = image.cols;
	int pixelsNum = rows * cols;
	int threadsNum = 1;//this->tp.getThreadsNum();
	if (threadsNum > pixelsNum) {
		std::cout << "The number of threads is bigger than the size of the image. Setting number of threads to size.\n";
		threadsNum = pixelsNum;
	}
	int sizeOfOneThread = pixelsNum / threadsNum;
	int remainder = pixelsNum % threadsNum;
	std::vector<std::shared_ptr<cv::Mat>> partsOfImage;

	this->tp.start();
	for (int i = 0; i < threadsNum; i++) {
		int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		partsOfImage.emplace_back(std::make_shared<cv::Mat>(rows, cols, image.type()));
		std::shared_ptr<cv::Mat> partOfImage = partsOfImage.back();
		this->tp.queueJob([this, partOfImage, firstIndex, lastIndex]() { this->filter(partOfImage, firstIndex, lastIndex); });
	}
	while (this->tp.busy()) {};
	this->tp.stop();
	for (int i = 0; i < threadsNum; i++) {
		int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		int partOfimageIndex = 0;
		int rowSize = this->data.rows;
		int colSize = this->data.cols;
		for (int j = firstIndex; j <= lastIndex; j++) {
			image.at<uchar>(j / colSize, j % colSize) = partsOfImage[i]->at<uchar>(j / colSize, j % colSize);
		}
	}
}

int getIndex(int startColIndex, int endColIndex, int rowIndex, int colsSize)
{
	return startColIndex * colsSize * colsSize + endColIndex * colsSize + rowIndex;
	//return startColIndex + endColIndex * colsSize + rowIndex * colsSize * colsSize;
}

std::optional<int> getDesiredValue(const std::vector<int>& prefixesPostfixes, int x, int y, int z, int colsSize) {
	auto value = prefixesPostfixes.at(getIndex(x, y, z, colsSize));
	return value == -1 ? std::nullopt : std::optional(value);
}

void SmartImageFilter::filter(std::shared_ptr<cv::Mat> newPartOfimage, int firstIndex, int lastIndex) {
	int imageSize = this->data.rows * this->data.cols;
	int rows = this->data.rows;
	int cols = this->data.cols;
	/*int firstIndexRow = firstIndex / cols;
	int firstIndexCol = firstIndex % cols;
	int lastIndexRow = lastIndex / rows;
	int lastIndexCol = lastIndex % cols;*/
	//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	int maskOneHalfLength = this->maskSize / 2;
	//Starting value is set in base class in regards to algType
	int targetValue = this->startingValue;
	auto t1 = std::chrono::high_resolution_clock::now();
	/*int lastRow = firstIndexRow;
	int lastCol = firstIndexCol;*/
	//[x][y][z]:
	//  x <- starting index of array in a row
	//  y <- end index of array in a row
	//  z <- row index

	// "i" is always the centre of first mask
	//std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>> prefixesPostfixes;
	//maskSize is also the height of mask
	//std::vector<int> prefixesPostfixes(cols * (1 + cols * (1 + rows)), -1);
	//std::vector<int> prefixesPostfixes(cols * cols * cols + cols * cols + rows, -1);
	//nie musze przetrzymywac wszystkich wartosci, jedynie dla aktualnej maski 
	std::vector<int> prefixesPostfixes(cols * cols * cols + cols * cols + rows, -1);
	auto lastRowIndex = 0;
	for (auto i = firstIndex; i <= lastIndex; i = std::min(i + maskSize + 1, lastIndex)) {
		auto rowIndex = i / cols;
		auto colIndex = i % cols;

		//nie jestem pewien czy to jest w koncu potrzebne? raczej powinno
		if (lastRowIndex < rowIndex) {
			lastRowIndex = rowIndex;
			i = cols * rowIndex;
			colIndex = 0;
		}

		//change name from leftmost to farleft
		auto firstMaskLeftMost = std::clamp(colIndex - maskOneHalfLength, 0, cols - 1);
		auto firstMaskRightMost = std::clamp(colIndex + maskOneHalfLength, 0, cols - 1);
		auto preSecondMaskCentre = std::clamp(colIndex + maskSize, 0, cols - 1);
		auto secondMaskLeftMost = std::clamp(preSecondMaskCentre - maskOneHalfLength, 0, cols - 1);
		auto secondMaskRightMost = std::clamp(preSecondMaskCentre + maskOneHalfLength, 0, cols - 1);
		auto topMost = std::clamp(rowIndex - maskOneHalfLength, 0, rows - 1);
		auto botMost = std::clamp(rowIndex + maskOneHalfLength, 0, rows - 1);

		for (auto j = topMost; j <= botMost; ++j) {
			for (auto k = firstMaskRightMost; k >= firstMaskLeftMost; --k)
				if (!getDesiredValue(prefixesPostfixes, k, firstMaskRightMost, j, cols)) {
					if (k == firstMaskRightMost) {
						int testValue = static_cast<int>(this->data.at<uchar>(j, k));
						prefixesPostfixes.at(getIndex(k, firstMaskRightMost, j, cols)) = testValue;
					}
					else {
						int currentValue = static_cast<int>(this->data.at<uchar>(j, k));
						int oldValue = prefixesPostfixes.at(getIndex(k + 1, firstMaskRightMost, j, cols));
						if (this->compare(currentValue, oldValue)) {
							prefixesPostfixes.at(getIndex(k, firstMaskRightMost, j, cols)) = currentValue;
						}
						else {
							prefixesPostfixes.at(getIndex(k, firstMaskRightMost, j, cols)) = oldValue;
						}
					}
				}


			for (auto k = secondMaskLeftMost; k <= secondMaskRightMost; ++k)
				if (!getDesiredValue(prefixesPostfixes, secondMaskLeftMost, k, j, cols)) {
					if (k == secondMaskLeftMost) {
						int testValue = static_cast<int>(this->data.at<uchar>(j, k));
						prefixesPostfixes.at(getIndex(secondMaskLeftMost, k, j, cols)) = testValue;
					}
					else {
						auto currentValue = static_cast<int>(this->data.at<uchar>(j, k));
						auto oldValue = prefixesPostfixes.at(getIndex(secondMaskLeftMost, k - 1, j, cols));
						if (this->compare(currentValue, oldValue)) {
							prefixesPostfixes.at(getIndex(secondMaskLeftMost, k, j, cols)) = currentValue;
						}
						else {
							prefixesPostfixes.at(getIndex(secondMaskLeftMost, k, j, cols)) = oldValue;
						}
					}
				}
		}

		auto firstMaskCentre = std::clamp(colIndex, firstMaskLeftMost, firstMaskRightMost);
		auto secondMaskCentre = std::clamp(colIndex + 2 * maskOneHalfLength + 1, secondMaskLeftMost, secondMaskRightMost);
		for (auto j = firstMaskCentre; j <= secondMaskCentre; ++j) {
			auto currentMaskLeftMost = std::clamp(j - maskOneHalfLength, firstMaskLeftMost, secondMaskRightMost);
			auto currentMaskRightMost = std::clamp(j + maskOneHalfLength, firstMaskLeftMost, secondMaskRightMost);
			if (j == firstMaskCentre)
			{
				auto bestValue = prefixesPostfixes.at(getIndex(firstMaskLeftMost, firstMaskRightMost, topMost, cols));
				for (auto k = topMost + 1; k <= botMost; ++k) {
					auto currentValue = prefixesPostfixes.at(getIndex(firstMaskLeftMost, firstMaskRightMost, k, cols));
					if (this->compare(currentValue, bestValue)) {
						bestValue = currentValue;
					}
				}
				newPartOfimage->at<uchar>(rowIndex, j) = static_cast<uchar>(bestValue);
			}
			else if (j == secondMaskCentre)
			{
				auto bestValue = prefixesPostfixes.at(getIndex(currentMaskLeftMost, currentMaskRightMost, topMost, cols));
				for (auto k = topMost + 1; k <= botMost; ++k) {
					auto currentValue = prefixesPostfixes.at(getIndex(currentMaskLeftMost, currentMaskRightMost, k, cols));
					if (this->compare(currentValue, bestValue)) {
						bestValue = currentValue;
					}
				}
				newPartOfimage->at<uchar>(rowIndex, j) = static_cast<uchar>(bestValue);
			}
			else if (auto diff = currentMaskRightMost - firstMaskRightMost; diff > 0) {
				auto firstPartOfMaskValue = prefixesPostfixes.at(getIndex(currentMaskLeftMost, firstMaskRightMost, topMost, cols));
				auto secondPartOfMaskValue = prefixesPostfixes.at(getIndex(secondMaskLeftMost, currentMaskRightMost, topMost, cols));
				int bestValue;
				if (this->compare(firstPartOfMaskValue, secondPartOfMaskValue)) {
					bestValue = firstPartOfMaskValue;
				}
				else {
					bestValue = secondPartOfMaskValue;
				}
				for (auto k = topMost + 1; k <= botMost; ++k) {
					firstPartOfMaskValue = prefixesPostfixes.at(getIndex(currentMaskLeftMost, firstMaskRightMost, k, cols));
					secondPartOfMaskValue = prefixesPostfixes.at(getIndex(secondMaskLeftMost, currentMaskRightMost, k, cols));
					if (this->compare(firstPartOfMaskValue, secondPartOfMaskValue)) {
						if (this->compare(firstPartOfMaskValue, bestValue)) {
							bestValue = firstPartOfMaskValue;
						}
					}
					else {
						if (this->compare(secondPartOfMaskValue, bestValue)) {
							bestValue = secondPartOfMaskValue;
						}
					}
				}
				newPartOfimage->at<uchar>(rowIndex, j) = static_cast<uchar>(bestValue);
			}
		}

		if (i == lastIndex)
			break;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << ms_double.count() << "ms\n";
}