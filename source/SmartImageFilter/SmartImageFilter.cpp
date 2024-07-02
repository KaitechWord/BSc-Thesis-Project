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
	for (auto i = firstIndex; i <= lastIndex; i = std::min(i + maskSize + 1, lastIndex)) {
		auto rowIndex = i / cols;
		auto colIndex = i & cols;

		//change name from leftmost to farleft
		auto firstMaskLeftMost = std::clamp(colIndex - maskOneHalfLength, 0, cols - 1);
		auto firstMaskRightMost = std::clamp(colIndex + maskOneHalfLength, 0, cols - 1);
		auto secondMaskLeftMost = std::clamp(firstMaskRightMost + 1, 0, cols - 1);
		auto secondMaskRightMost = std::clamp(firstMaskRightMost + this->maskSize, 0, cols - 1);
		auto topMost = std::clamp(rowIndex - maskOneHalfLength, 0, rows - 1);
		auto botMost = std::clamp(rowIndex + maskOneHalfLength, 0, rows - 1);

		for (auto j = topMost; j <= botMost; ++j) {
			for (auto k = firstMaskRightMost; k >= firstMaskLeftMost; --k)
				if (!getDesiredValue(desiredValues, firstMaskRightMost, k, j))
					if (k == firstMaskRightMost)
						desiredValues[firstMaskRightMost][k][j] = static_cast<int>(this->data.at<uchar>(j, k));
					else
						desiredValues[firstMaskRightMost][k][j] = this->compare(static_cast<int>(this->data.at<uchar>(j, k)), desiredValues[firstMaskRightMost][k + 1][j]);

			for (auto k = secondMaskLeftMost; k <= secondMaskRightMost; ++k)
				if (!getDesiredValue(desiredValues, k, secondMaskRightMost, j))
					if (k == secondMaskLeftMost)
						desiredValues[k][secondMaskRightMost][j] = static_cast<int>(this->data.at<uchar>(j, k));
					else
						desiredValues[k][secondMaskRightMost][j] = this->compare(static_cast<int>(this->data.at<uchar>(j, k)), desiredValues[k - 1][secondMaskRightMost][j]);
		}

		//add calculating the processed pixels from firstMaskCenter(i) to sceondMaskCentre
		auto secondMaskCentre = std::clamp(secondMaskLeftMost + maskOneHalfLength, firstMaskLeftMost, secondMaskRightMost);
		for (auto j = i; j <= secondMaskCentre; ++j) {
			newPartOfimage->at<uchar>(rowIndex, j) = static_cast<uchar>(desiredValues[j][firstMaskRightMost + j][rowIndex]);
		}

		if (i == lastIndex)
			break;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << ms_double.count() << "ms\n";
}