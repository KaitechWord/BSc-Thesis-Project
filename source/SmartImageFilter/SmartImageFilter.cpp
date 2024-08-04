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
	int threadsNum = 100;//this->tp.getThreadsNum();
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
		for (int j = firstIndex; j <= lastIndex; j++) {
			image.at<uchar>(j / rowSize, j % rowSize) = partsOfImage[i]->at<uchar>(j / rowSize, j % rowSize);
		}
	}
}

std::optional<int> getDesiredValue(std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>>& desiredValues, int x, int y, int z) {
	auto itX = desiredValues.find(x);
	if (itX != desiredValues.end()) {
		auto itY = itX->second.find(y);
		if (itY != itX->second.end()) {
			auto itZ = itY->second.find(z);
			if (itZ != itY->second.end()) {
				return itZ->second;  // Found the value
			}
		}
	}
	return std::nullopt;  // Value not found
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
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>> desiredValues;
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
				if (!getDesiredValue(desiredValues, k, firstMaskRightMost, j)) {
					if (k == firstMaskRightMost) {
						int testValue = static_cast<int>(this->data.at<uchar>(j, k));
						desiredValues[k][firstMaskRightMost][j] = testValue;
					}
					else {
						int currentValue = static_cast<int>(this->data.at<uchar>(j, k));
						int oldValue = desiredValues.at(k + 1).at(firstMaskRightMost).at(j);
						if (this->compare(currentValue, oldValue)) {
							desiredValues[k][firstMaskRightMost][j] = currentValue;
						}
						else {
							desiredValues[k][firstMaskRightMost][j] = oldValue;
						}
					}
				}


			for (auto k = secondMaskLeftMost; k <= secondMaskRightMost; ++k)
				if (!getDesiredValue(desiredValues, secondMaskLeftMost, k, j)) {
					if (k == secondMaskLeftMost) {
						int testValue = static_cast<int>(this->data.at<uchar>(j, k));
						desiredValues[secondMaskLeftMost][k][j] = testValue;
					}
					else {
						auto currentValue = static_cast<int>(this->data.at<uchar>(j, k));
						auto oldValue = desiredValues.at(secondMaskLeftMost).at(k - 1).at(j);
						if (this->compare(currentValue, oldValue)) {
							desiredValues[secondMaskLeftMost][k][j] = currentValue;
						}
						else {
							desiredValues[secondMaskLeftMost][k][j] = oldValue;
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
				auto bestValue = desiredValues.at(firstMaskLeftMost).at(firstMaskRightMost).at(topMost);
				for (auto k = topMost + 1; k <= botMost; ++k) {
					auto currentValue = desiredValues.at(firstMaskLeftMost).at(firstMaskRightMost).at(k);
					if (this->compare(currentValue, bestValue)) {
						bestValue = currentValue;
					}
				}
				newPartOfimage->at<uchar>(rowIndex, j) = static_cast<uchar>(bestValue);
			}
			else if (j == secondMaskCentre)
			{
				auto bestValue = desiredValues.at(currentMaskLeftMost).at(currentMaskRightMost).at(topMost);
				for (auto k = topMost + 1; k <= botMost; ++k) {
					auto currentValue = desiredValues.at(currentMaskLeftMost).at(currentMaskRightMost).at(k);
					if (this->compare(currentValue, bestValue)) {
						bestValue = currentValue;
					}
				}
				newPartOfimage->at<uchar>(rowIndex, j) = static_cast<uchar>(bestValue);
			}
			else if (auto diff = currentMaskRightMost - firstMaskRightMost; diff > 0) {
				auto firstPartOfMaskValue = desiredValues.at(currentMaskLeftMost).at(firstMaskRightMost).at(topMost);
				auto secondPartOfMaskValue = desiredValues.at(secondMaskLeftMost).at(currentMaskRightMost).at(topMost);
				int bestValue;
				if (this->compare(firstPartOfMaskValue, secondPartOfMaskValue)) {
					bestValue = firstPartOfMaskValue;
				}
				else {
					bestValue = secondPartOfMaskValue;
				}
				for (auto k = topMost + 1; k <= botMost; ++k) {
					firstPartOfMaskValue = desiredValues.at(currentMaskLeftMost).at(firstMaskRightMost).at(k);
					secondPartOfMaskValue = desiredValues.at(secondMaskLeftMost).at(currentMaskRightMost).at(k);
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