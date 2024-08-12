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
	int threadsNum = 13;//this->tp.getThreadsNum();
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
		int colSize = this->data.cols;
		for (int j = firstIndex; j <= lastIndex; j++) {
			image.at<uchar>(j / colSize, j % colSize) = partsOfImage[i]->at<uchar>(j / colSize, j % colSize);
		}
	}
}

//2D array -> flatten array
// indexing:
//          [y][x] -> y * xMaxSize + x //we move in row from top to down
//			[x][y] -> x * yMaxSize + y //we move in column from top to down
//3D array -> flatten array
// indexing:
//          [z][y][x] -> z * yMaxSize * xMaxSize + y * xMaxSize + x
//			let's assume: x - first mask index column, y - last mask index column, z - rowIndex
//			[x][y][z] -> x * yMaxSize * zMaxSize + y * zMaxSize + z
//					^ this is faster coz zMaxSize is the size of the mask, which, supposedly, is much lower than number of columns
//          [z][y][x] -> z * yMaxSize * xMaxSize + y * xMaxSize + x
//					^ this is slower coz xMaxSize/yMaxSize is the number of columns, which should be bigger than mask size

inline int getIndex(int startColIndex, int endColIndex, int rowIndex, int colsSize, int maskSize)
{
	return startColIndex * maskSize * maskSize + endColIndex * maskSize + rowIndex;
	//return startColIndex * colsSize * colsSize + endColIndex * colsSize + rowIndex;
	//return startColIndex + endColIndex * colsSize + rowIndex * colsSize * colsSize;
}

//std::optional<uint8_t> getDesiredValue(const std::vector<uint8_t>& prefixesPostfixes, int x, int y, int z, int colsSize, int maskSize) {
//	auto value = prefixesPostfixes.at(getIndex(x, y, z, colsSize, maskSize));
//	return value == -1 ? std::nullopt : std::optional(value);
//}

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

	// IMPORTANT
	// USE uchar/uuint8_t LATER TO SAVE MEMORY or int_16t to store -1 to indicate that it has not been calculated yer
	// IMPORTANT


	// IMPORTANT
	// IMPORTANT
	// IMPORTANT
	// j x k x l:
	// j - firstIndexOfPrefixOrPostfix, k - secondIndexOfPrefixOrPostfix, l - rowIndex
	// instead of storing 256 x 256 x 5
	// where 256 is cols and 5 is maskSize
	// and the preserving space like that in an array:
	// std::vector<uint8_t> prefixesPostfixes(maskSize * cols * cols + cols * maskSize + maskSize);
	// you should store 
	// q x w x e
	// q - firstIndexOfPrefixOrPostfix, w - offset, in general, what should be add to q to get the secondIndexOfPrefixOrPostfix, l - rowIndex
	// maxSizes:
	// q - cols, w - maskSize, e - maskSize
	// Implement this behaviour and check the correctness
	// IMPORTANT
	// IMPORTANT
	// IMPORTANT

	// "i" is always the centre of first mask
	//std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, int>>> prefixesPostfixes;
	//maskSize is also the height of mask
	//std::vector<int> prefixesPostfixes(cols * (1 + cols * (1 + rows)), -1);
	//std::vector<int> prefixesPostfixes(cols * cols * cols + cols * cols + rows, -1);
	//nie musze przetrzymywac wszystkich wartosci, jedynie dla aktualnej maski 
	//wspomij o uint8_t
	std::vector<uchar> prefixesPostfixes(cols * maskSize * maskSize);
	auto lastRowIndex = 0;
	for (auto i = firstIndex; i <= lastIndex; i = std::min(i + maskSize + 1, lastIndex)) {
		auto rowIndex = i / cols;
		auto colIndex = i % cols;

		if (lastRowIndex < rowIndex) {
			lastRowIndex = rowIndex;
			i = cols * rowIndex;
			colIndex = 0;
		}

		auto firstMaskFarLeftIndex = std::clamp(colIndex - maskOneHalfLength, 0, cols - 1);
		auto firstMaskFarRightIndex = std::clamp(colIndex + maskOneHalfLength, 0, cols - 1);
		auto presecondMaskCentreIndex = std::clamp(colIndex + maskSize, 0, cols - 1);
		auto secondMaskFarLeftIndex = std::clamp(presecondMaskCentreIndex - maskOneHalfLength, 0, cols - 1);
		auto secondMaskFarRightIndex = std::clamp(presecondMaskCentreIndex + maskOneHalfLength, 0, cols - 1);
		auto farTopIndex = std::clamp(rowIndex - maskOneHalfLength, 0, rows - 1);
		auto farBotIndex = std::clamp(rowIndex + maskOneHalfLength, 0, rows - 1);

		for (auto j = farTopIndex; j <= farBotIndex; ++j) {
			prefixesPostfixes.at(getIndex(firstMaskFarRightIndex, 0, j % maskSize, cols, maskSize)) = this->data.at<uchar>(j, firstMaskFarRightIndex);
			for (auto k = firstMaskFarRightIndex - 1; k >= firstMaskFarLeftIndex; --k) {
				auto currentValue = this->data.at<uchar>(j, k);
				auto oldValue = prefixesPostfixes.at(getIndex(k + 1, firstMaskFarRightIndex - (k + 1), j % maskSize, cols, maskSize));
				if (this->compare(currentValue, oldValue)) {
					prefixesPostfixes.at(getIndex(k, firstMaskFarRightIndex - k, j % maskSize, cols, maskSize)) = currentValue;
				}
				else {
					prefixesPostfixes.at(getIndex(k, firstMaskFarRightIndex - k, j % maskSize, cols, maskSize)) = oldValue;
				}
			}

			prefixesPostfixes.at(getIndex(secondMaskFarLeftIndex, 0, j % maskSize, cols, maskSize)) = this->data.at<uchar>(j, secondMaskFarLeftIndex);
			for (auto k = secondMaskFarLeftIndex + 1; k <= secondMaskFarRightIndex; ++k) {
				auto currentValue = this->data.at<uchar>(j, k);
				auto oldValue = prefixesPostfixes.at(getIndex(secondMaskFarLeftIndex, k - 1 - secondMaskFarLeftIndex, j % maskSize, cols, maskSize));
				if (this->compare(currentValue, oldValue)) {
					prefixesPostfixes.at(getIndex(secondMaskFarLeftIndex, k - secondMaskFarLeftIndex, j % maskSize, cols, maskSize)) = currentValue;
				}
				else {
					prefixesPostfixes.at(getIndex(secondMaskFarLeftIndex, k - secondMaskFarLeftIndex, j % maskSize, cols, maskSize)) = oldValue;
				}
			}
		}

		auto firstMaskCentreIndex = std::clamp(colIndex, firstMaskFarLeftIndex, firstMaskFarRightIndex);
		auto secondMaskCentreIndex = std::clamp(colIndex + 2 * maskOneHalfLength + 1, secondMaskFarLeftIndex, secondMaskFarRightIndex);

		auto bestValue = prefixesPostfixes.at(getIndex(firstMaskFarLeftIndex, firstMaskFarRightIndex - firstMaskFarLeftIndex, farTopIndex % maskSize, cols, maskSize));
		for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
			auto currentValue = prefixesPostfixes.at(getIndex(firstMaskFarLeftIndex, firstMaskFarRightIndex - firstMaskFarLeftIndex, k % maskSize, cols, maskSize));
			if (this->compare(currentValue, bestValue)) {
				bestValue = currentValue;
			}
		}
		newPartOfimage->at<uchar>(rowIndex, firstMaskCentreIndex) = bestValue;
		for (auto j = firstMaskCentreIndex + 1; j < secondMaskCentreIndex; ++j) {
			auto currentMaskLeftMost = std::clamp(j - maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
			auto currentMaskRightMost = std::clamp(j + maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
			auto firstPartOfMaskValue = prefixesPostfixes.at(getIndex(currentMaskLeftMost, firstMaskFarRightIndex - currentMaskLeftMost, farTopIndex % maskSize, cols, maskSize));
			auto secondPartOfMaskValue = prefixesPostfixes.at(getIndex(secondMaskFarLeftIndex, currentMaskRightMost - secondMaskFarLeftIndex, farTopIndex % maskSize, cols, maskSize));
			int bestValue;
			if (this->compare(firstPartOfMaskValue, secondPartOfMaskValue)) {
				bestValue = firstPartOfMaskValue;
			}
			else {
				bestValue = secondPartOfMaskValue;
			}
			for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
				firstPartOfMaskValue = prefixesPostfixes.at(getIndex(currentMaskLeftMost, firstMaskFarRightIndex - currentMaskLeftMost, k % maskSize, cols, maskSize));
				secondPartOfMaskValue = prefixesPostfixes.at(getIndex(secondMaskFarLeftIndex, currentMaskRightMost - secondMaskFarLeftIndex, k % maskSize, cols, maskSize));
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
			newPartOfimage->at<uchar>(rowIndex, j) = bestValue;
		}
		auto currentMaskLeftMost = std::clamp(secondMaskCentreIndex - maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
		auto currentMaskRightMost = std::clamp(secondMaskCentreIndex + maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
		bestValue = prefixesPostfixes.at(getIndex(currentMaskLeftMost, currentMaskRightMost - currentMaskLeftMost, farTopIndex % maskSize, cols, maskSize));
		for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
			auto currentValue = prefixesPostfixes.at(getIndex(currentMaskLeftMost, currentMaskRightMost - currentMaskLeftMost, k % maskSize, cols, maskSize));
			if (this->compare(currentValue, bestValue)) {
				bestValue = currentValue;
			}
		}
		newPartOfimage->at<uchar>(rowIndex, secondMaskCentreIndex) = bestValue;

		if (i == lastIndex)
			break;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms_double = t2 - t1;
	std::cout << ms_double.count() << "ms\n";
}