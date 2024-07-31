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
		for (int j = firstIndex; j <= lastIndex; j++) {
			image.at<uchar>(j / rowSize, j % rowSize) = partsOfImage[i]->at<uchar>(j / rowSize, j % rowSize);
		}
	}
}

std::optional<int> getDesiredValue(std::map<int, std::map<int, std::map<int, int>>>& desiredValues, int x, int y, int z) {
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

void checkZero(const std::map<int, std::map<int, std::map<int, int>>>& desiredValues)
{
	for (const auto& topPair : desiredValues) {
		const auto& middleMap = topPair.second;

		// Iterate through the middle-level map
		for (const auto& middlePair : middleMap) {
			const auto& deepestMap = middlePair.second;

			// Check if key3 exists at the deepest level
			for (const auto& deepestPair : deepestMap) {
				if (deepestPair.second == 0) {
					std::cout << deepestPair.second << " found at: [startIndex] = " << topPair.first << " [lastIndex] = " << middlePair.first << " [depth] = " << deepestPair.first << std::endl;
				}
			}
		}
	}
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
	std::map<int, std::map<int, std::map<int, int>>> desiredValues;
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
		auto secondMaskLeftMost = std::clamp(firstMaskRightMost + 1, 0, cols - 1);
		auto secondMaskRightMost = std::clamp(firstMaskRightMost + this->maskSize, 0, cols - 1);
		auto topMost = std::clamp(rowIndex - maskOneHalfLength, 0, rows - 1);
		auto botMost = std::clamp(rowIndex + maskOneHalfLength, 0, rows - 1);

		for (auto j = topMost; j <= botMost; ++j) {
			for (auto k = firstMaskRightMost; k >= firstMaskLeftMost; --k)
				if (!getDesiredValue(desiredValues, k, firstMaskRightMost, j)) {
					if (k == firstMaskRightMost) {
						int testValue = static_cast<int>(this->data.at<uchar>(j, k));
						desiredValues[k][firstMaskRightMost][j] = testValue;
						checkZero(desiredValues);
					}
					else {
						int currentValue = static_cast<int>(this->data.at<uchar>(j, k));
						int oldValue = desiredValues.at(k + 1).at(firstMaskRightMost).at(j);
						if (this->compare(currentValue, oldValue)) {
							desiredValues[k][firstMaskRightMost][j] = currentValue;
							checkZero(desiredValues);
						}
						else {
							desiredValues[k][firstMaskRightMost][j] = oldValue;
							checkZero(desiredValues);
						}
					}
				}


			for (auto k = secondMaskLeftMost; k <= secondMaskRightMost; ++k)
				if (!getDesiredValue(desiredValues, secondMaskLeftMost, k, j)) {
					if (k == secondMaskLeftMost) {
						int testValue = static_cast<int>(this->data.at<uchar>(j, k));
						desiredValues[secondMaskLeftMost][k][j] = testValue;
						checkZero(desiredValues);
					}
					else {
						auto currentValue = static_cast<int>(this->data.at<uchar>(j, k));
						auto oldValue = desiredValues.at(secondMaskLeftMost).at(k - 1).at(j);
						if (this->compare(currentValue, oldValue)) {
							desiredValues[secondMaskLeftMost][k][j] = currentValue;
							checkZero(desiredValues);
						}
						else {
							desiredValues[secondMaskLeftMost][k][j] = oldValue;
							checkZero(desiredValues);
						}
					}
				}
		}

		//add calculating the processed pixels from firstMaskCenter(i) to sceondMaskCentre
		//tutaj jest zle, bo krancowe wartosci sie wywracaja, nie sprawdzamy ich
		//tez jest problem bo w ciekawy sposob zapisuje do desiredValues (dla pierwszej maski od konca tej maski, a dla drugiej od jej poczatku)
		//ponizszego for loopa tez moznaby zapisac z rozroznieniem dla pierwszej i drugiej maski z tego powodu? albo w normalny sposob zapisywac jest do desiredValues

		//isZeroDetected <- na podstawie tego ponizej ogarnij czemu jest zero
		//for (const auto& topPair : desiredValues) {
		//	const auto& middleMap = topPair.second;

		//	// Iterate through the middle-level map
		//	for (const auto& middlePair : middleMap) {
		//		const auto& deepestMap = middlePair.second;

		//		// Check if key3 exists at the deepest level
		//		for (const auto& deepestPair : deepestMap) {
		//			if (deepestPair.second == 0) {
		//				std::cout << deepestPair.second << " found at: [startIndex] = " << topPair.first << " [lastIndex] = " << middlePair.first << " [depth] = " << deepestPair.first << std::endl;
		//			}
		//		}
		//	}
		//}

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
				auto bestValue = desiredValues.at(secondMaskLeftMost).at(secondMaskRightMost).at(topMost);
				for (auto k = topMost + 1; k <= botMost; ++k) {
					auto currentValue = desiredValues.at(secondMaskLeftMost).at(secondMaskRightMost).at(k);
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

		/*auto firstMaskCentre = std::clamp(i, firstMaskLeftMost, firstMaskRightMost);
		auto secondMaskCentre = std::clamp(i + 2 * maskOneHalfLength + 1, secondMaskLeftMost, secondMaskRightMost);
		for (auto j = firstMaskCentre; j <= secondMaskCentre; ++j) {
			auto currentMaskLeftMost = std::clamp(j - maskOneHalfLength, firstMaskLeftMost, secondMaskRightMost);
			auto currentMaskRightMost = std::clamp(j + maskOneHalfLength, firstMaskLeftMost, secondMaskRightMost);
			if (auto diff = currentMaskRightMost - firstMaskRightMost; diff > 0) {
				int firstPartOfMaskValue = 200;
				int secondPartOfMaskValue = 200;
				if (getDesiredValue(desiredValues, currentMaskLeftMost, firstMaskRightMost, topMost))
					firstPartOfMaskValue = desiredValues[currentMaskLeftMost][firstMaskRightMost][topMost];
				else
					std::cout << "didnt find at " << currentMaskLeftMost << " " << firstMaskRightMost << " " << topMost << std::endl;
				if(getDesiredValue(desiredValues, firstMaskRightMost + diff, currentMaskRightMost, topMost))
					secondPartOfMaskValue = desiredValues[firstMaskRightMost + diff][currentMaskRightMost][topMost];
				else
					std::cout << "didnt find at " << firstMaskRightMost + diff << " " << currentMaskRightMost << " " << topMost << std::endl;
				int bestValue;
				if (this->compare(firstPartOfMaskValue, secondPartOfMaskValue)) {
					bestValue = firstPartOfMaskValue;
				}
				else {
					bestValue = secondPartOfMaskValue;
				}
				for (auto k = topMost + 1; k <= botMost; ++k) {
					firstPartOfMaskValue = desiredValues[currentMaskLeftMost][firstMaskRightMost][k];
					secondPartOfMaskValue = desiredValues[firstMaskRightMost + diff][currentMaskRightMost][k];
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
				continue;
			}
			auto bestValue = desiredValues.at(currentMaskLeftMost).at(currentMaskRightMost).at(topMost);
			for (auto k = topMost + 1; k <= botMost; ++k) {
				auto currentValue = desiredValues.at(currentMaskLeftMost).at(currentMaskRightMost).at(k);
				if (this->compare(currentValue, bestValue)) {
					bestValue = currentValue;
				}
			}
			newPartOfimage->at<uchar>(rowIndex, j) = static_cast<uchar>(bestValue);
		}*/

		if (i == lastIndex)
			break;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << ms_double.count() << "ms\n";
}