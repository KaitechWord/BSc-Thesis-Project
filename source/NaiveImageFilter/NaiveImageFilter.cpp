#include "NaiveImageFilter.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>

static const auto minSingleTextFile = std::string(ROOT_DIR) + "/output/NaiveMinSingle.txt";
static const auto minMultiTextFile = std::string(ROOT_DIR) + "/output/NaiveMinMulti.txt";
static const auto maxSingleTextFile = std::string(ROOT_DIR) + "/output/NaiveMaxSingle.txt";
static const auto maxMultiTextFile = std::string(ROOT_DIR) + "/output/NaiveMaxMulti.txt";

NaiveImageFilter::NaiveImageFilter(int threadNum, AlgorithmType algType, int maskSize)
	: ImageFilter(threadNum, algType, maskSize)
{}

void NaiveImageFilter::apply(cv::Mat& image) {
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
		threads.emplace_back(std::thread(&NaiveImageFilter::filter, this, std::ref(newImage), firstIndex, lastIndex));
	}
	for (auto& thread : threads)
		thread.join();
	auto end = std::chrono::high_resolution_clock::now();
	const auto execTime = std::chrono::duration<double, std::milli>(end - start).count();
	std::cout << "Naive " << (this->algType == AlgorithmType::MIN ? "min." : "max.") << " image " << (threadsNum == 1 ? "(single-threaded)" : "(multi-threaded)") << " filter execution time : " << execTime << "ms.\n";
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
		auto rowSize = this->data.rows;
		auto colSize = this->data.cols;
		for (auto j = firstIndex; j <= lastIndex; ++j) {
			auto rowIndex = j / colSize;
			auto colIndex = j % colSize;
			image.at<uchar>(rowIndex, colIndex) = newImage.at<uchar>(rowIndex, colIndex);
		}
	}
	outfile.close();	
}

void NaiveImageFilter::filter(cv::Mat& newImage, int firstIndex, int lastIndex) {
	auto imageSize = this->data.rows * this->data.cols;
	auto rowSize = this->data.rows;
	auto colSize = this->data.cols;
	auto firstIndexRow = firstIndex / colSize;
	auto firstIndexCol = firstIndex % colSize;
	auto lastIndexRow = lastIndex / colSize;
	auto lastIndexCol = lastIndex % colSize;
	//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	auto maskOneHalfLength = this->maskSize / 2;
	//Starting value is set in base class in regards to algType
	auto targetValue = this->startingValue;
	auto rowOfTargetValue = std::min(firstIndexRow - maskOneHalfLength, rowSize);
	auto colOfTargetValue = std::max(0, firstIndexCol - maskOneHalfLength);
	auto lastRow = firstIndexRow;
	auto lastCol = firstIndexCol;

	//First iteration
	auto rowIndex = firstIndex / colSize;
	auto colIndex = firstIndex % colSize;
	auto leftMostIndexOfMask = std::max(0, colIndex - maskOneHalfLength);
	auto rightMostIndexOfMask = std::min(colIndex + maskOneHalfLength, colSize - 1);
	auto topMostIndexOfMask = std::max(0, rowIndex - maskOneHalfLength);
	auto botMostIndexOfMask = std::min(rowIndex + maskOneHalfLength, rowSize - 1);
	for (auto k = topMostIndexOfMask; k <= botMostIndexOfMask; ++k) {
		for (auto l = leftMostIndexOfMask; l <= rightMostIndexOfMask; ++l) {
			if (this->compare(static_cast<int>(this->data.at<uchar>(k, l)), targetValue)) {
				targetValue = static_cast<int>(this->data.at<uchar>(k, l));
				rowOfTargetValue = k;
				colOfTargetValue = l;
			}
		}
	}
	auto lastLeftMostIndexOfMask = leftMostIndexOfMask;
	auto lastRightMostIndexOfMask = rightMostIndexOfMask;
	auto lastBotMostIndexOfMask = botMostIndexOfMask;

	newImage.at<uchar>(rowIndex, colIndex) = static_cast<uchar>(targetValue);

	for (auto i = firstIndex + 1; i <= lastIndex; ++i) {
		++colIndex;
		if (colIndex == colSize) {
			colIndex = 0;
			++rowIndex;
		}

		auto leftMostIndexOfMask = std::max(0, colIndex - maskOneHalfLength);
		auto rightMostIndexOfMask = std::min(colIndex + maskOneHalfLength, colSize - 1);
		auto topMostIndexOfMask = std::max(0, rowIndex - maskOneHalfLength);
		auto botMostIndexOfMask = std::min(rowIndex + maskOneHalfLength, rowSize - 1);

		//If last target value is outside the mask, we need to check every pixel of new mask
		const auto isLastTargetValueOutsideOfNewMask = colOfTargetValue < leftMostIndexOfMask ||
			colOfTargetValue > rightMostIndexOfMask ||
			rowOfTargetValue < topMostIndexOfMask ||
			rowOfTargetValue > botMostIndexOfMask;
		if (isLastTargetValueOutsideOfNewMask) {
			targetValue = this->startingValue;
			for (auto k = topMostIndexOfMask; k <= botMostIndexOfMask; ++k) {
				for (auto l = leftMostIndexOfMask; l <= rightMostIndexOfMask; ++l) {
					if (this->compare(static_cast<int>(this->data.at<uchar>(k, l)), targetValue)) {
						targetValue = static_cast<int>(this->data.at<uchar>(k, l));
						rowOfTargetValue = k;
						colOfTargetValue = l;
					}
				}
			}
		}
		else {
			//If row changed, we need to check every pixel that is not overlapping with previous mask to find the new minimum and we check if the row really changed
			//or we are shortening the mask once again due to the bottom border
			if (lastBotMostIndexOfMask < botMostIndexOfMask) {
				for (auto k = topMostIndexOfMask; k < botMostIndexOfMask; ++k) {
					for (auto l = leftMostIndexOfMask; l < lastLeftMostIndexOfMask; ++l) {
						if (this->compare(static_cast<int>(this->data.at<uchar>(k, l)), targetValue)) {
							targetValue = static_cast<int>(this->data.at<uchar>(k, l));
							rowOfTargetValue = k;
							colOfTargetValue = l;
						}
					}
				}
				for (auto l = leftMostIndexOfMask; l < rightMostIndexOfMask; ++l) {
					if (this->compare(static_cast<int>(this->data.at<uchar>(botMostIndexOfMask, l)), targetValue)) {
						targetValue = static_cast<int>(this->data.at<uchar>(botMostIndexOfMask, l));
						rowOfTargetValue = botMostIndexOfMask;
						colOfTargetValue = l;
					}
				}
			}



			//If row didn't change, we know that col must have changed, but we need to check if there is really a new col or we are just shortening the mask
			//due to the fact that right image border stops us and if current right most index is greater than the last one, we need to check the new column that entered the mask
			if (lastRightMostIndexOfMask < rightMostIndexOfMask) {
				for (auto k = topMostIndexOfMask; k <= botMostIndexOfMask; ++k) {
					if (this->compare(static_cast<int>(this->data.at<uchar>(k, rightMostIndexOfMask)), targetValue)) {
						targetValue = static_cast<int>(this->data.at<uchar>(k, rightMostIndexOfMask));
						rowOfTargetValue = k;
						colOfTargetValue = rightMostIndexOfMask;
					}
				}
			}
		}
		lastLeftMostIndexOfMask = leftMostIndexOfMask;
		lastRightMostIndexOfMask = rightMostIndexOfMask;
		lastBotMostIndexOfMask = botMostIndexOfMask;

		newImage.at<uchar>(rowIndex, colIndex) = static_cast<uchar>(targetValue);
	}
}