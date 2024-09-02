#include "SmartImageFilter.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <thread>

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

inline int getIndex(int startColIndex, int endColIndex, int rowIndex, int colsSize, int maskSize)
{
	//maskSize * maskSize wyeliminowac - jedna zmienna
	return startColIndex * maskSize * maskSize + endColIndex * maskSize + rowIndex;
}

void SmartImageFilter::filter(cv::Mat& newImage, int firstIndex, int lastIndex) {
	auto imageSize = this->data.rows * this->data.cols;
	auto rows = this->data.rows;
	auto cols = this->data.cols;
	//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	auto maskOneHalfLength = this->maskSize / 2;
	//Starting value is set in base class in regards to algType
	auto targetValue = this->startingValue;
	//Flatten 3D array
	std::vector<uchar> prefixesPostfixes(cols * maskSize * maskSize);
	auto lastRowIndex = 0;
	// "i" is always the centre of first mask
	for (auto i = firstIndex; i <= lastIndex; i = std::min(i + maskSize + 1, lastIndex)) {
		//sprytniejsze obliczenie rowIndex i colIndex (dzielenie i modulo jest kozstowne)
		auto rowIndex = i / cols;
		auto colIndex = i % cols;

		//pomyslec nad tym aby w i = std::min(i + maskSize + 1, lastIndex) bylo prawidlowo obliczane
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
			//pozbyc sie modulo na rzecz ifa (++ z poprzedniej wartosci j)
			prefixesPostfixes[getIndex(firstMaskFarRightIndex, 0, j % maskSize, cols, maskSize)] = this->data.at<uchar>(j, firstMaskFarRightIndex);
			//Prefixes - first mask
			for (auto k = firstMaskFarRightIndex - 1; k >= firstMaskFarLeftIndex; --k) {
				auto currentValue = this->data.at<uchar>(j, k);
				auto oldValue = prefixesPostfixes[getIndex(k + 1, firstMaskFarRightIndex - (k + 1), j % maskSize, cols, maskSize)];
				if (this->compare(currentValue, oldValue)) {
					prefixesPostfixes[getIndex(k, firstMaskFarRightIndex - k, j % maskSize, cols, maskSize)] = currentValue;
				}
				else {
					prefixesPostfixes[getIndex(k, firstMaskFarRightIndex - k, j % maskSize, cols, maskSize)] = oldValue;
				}
			}

			prefixesPostfixes[getIndex(secondMaskFarLeftIndex, 0, j % maskSize, cols, maskSize)] = this->data.at<uchar>(j, secondMaskFarLeftIndex);
			//Postfixes - secondmask
			for (auto k = secondMaskFarLeftIndex + 1; k <= secondMaskFarRightIndex; ++k) {
				auto currentValue = this->data.at<uchar>(j, k);
				auto oldValue = prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - 1 - secondMaskFarLeftIndex, j % maskSize, cols, maskSize)];
				if (this->compare(currentValue, oldValue)) {
					prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - secondMaskFarLeftIndex, j % maskSize, cols, maskSize)] = currentValue;
				}
				else {
					prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - secondMaskFarLeftIndex, j % maskSize, cols, maskSize)] = oldValue;
				}
			}
		}

		auto firstMaskCentreIndex = std::clamp(colIndex, firstMaskFarLeftIndex, firstMaskFarRightIndex);
		auto secondMaskCentreIndex = std::clamp(colIndex + 2 * maskOneHalfLength + 1, secondMaskFarLeftIndex, secondMaskFarRightIndex);

		//Finding best value for left-first index from first mask (it is separate case - we need to only use prefixes from first mask)
		//Iterating from top row of mask up to bottom row of mask
		auto bestValue = prefixesPostfixes[getIndex(firstMaskFarLeftIndex, firstMaskFarRightIndex - firstMaskFarLeftIndex, farTopIndex % maskSize, cols, maskSize)];
		for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
			auto currentValue = prefixesPostfixes[getIndex(firstMaskFarLeftIndex, firstMaskFarRightIndex - firstMaskFarLeftIndex, k % maskSize, cols, maskSize)];
			if (this->compare(currentValue, bestValue)) {
				bestValue = currentValue;
			}
		}
		newImage.at<uchar>(rowIndex, firstMaskCentreIndex) = bestValue;
		//Finding best value for indices that are mix of prefixes from first mask and postfixes from second mask
		for (auto j = firstMaskCentreIndex + 1; j < secondMaskCentreIndex; ++j) {
			auto currentMaskLeftMost = std::clamp(j - maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
			auto currentMaskRightMost = std::clamp(j + maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
			auto firstPartOfMaskValue = prefixesPostfixes[getIndex(currentMaskLeftMost, firstMaskFarRightIndex - currentMaskLeftMost, farTopIndex % maskSize, cols, maskSize)];
			auto secondPartOfMaskValue = prefixesPostfixes[getIndex(secondMaskFarLeftIndex, currentMaskRightMost - secondMaskFarLeftIndex, farTopIndex % maskSize, cols, maskSize)];
			int bestValue;
			if (this->compare(firstPartOfMaskValue, secondPartOfMaskValue)) {
				bestValue = firstPartOfMaskValue;
			}
			else {
				bestValue = secondPartOfMaskValue;
			}
			//Iterating from top row of mask up to bottom row of mask
			for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
				firstPartOfMaskValue = prefixesPostfixes[getIndex(currentMaskLeftMost, firstMaskFarRightIndex - currentMaskLeftMost, k % maskSize, cols, maskSize)];
				secondPartOfMaskValue = prefixesPostfixes[getIndex(secondMaskFarLeftIndex, currentMaskRightMost - secondMaskFarLeftIndex, k % maskSize, cols, maskSize)];
				//na sztywno std::min / std::max jedno z tych - sprobowac
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
			newImage.at<uchar>(rowIndex, j) = bestValue;
		}
		auto currentMaskLeftMost = std::clamp(secondMaskCentreIndex - maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
		auto currentMaskRightMost = std::clamp(secondMaskCentreIndex + maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
		//Finding best value for last index from second mask (it is separate case - we need to only use postfixes from second mask)
		//Iterating from top row of mask up to bottom row of mask
		bestValue = prefixesPostfixes[getIndex(currentMaskLeftMost, currentMaskRightMost - currentMaskLeftMost, farTopIndex % maskSize, cols, maskSize)];
		for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
			auto currentValue = prefixesPostfixes[getIndex(currentMaskLeftMost, currentMaskRightMost - currentMaskLeftMost, k % maskSize, cols, maskSize)];
			if (this->compare(currentValue, bestValue)) {
				bestValue = currentValue;
			}
		}
		newImage.at<uchar>(rowIndex, secondMaskCentreIndex) = bestValue;

		if (i == lastIndex)
			break;
	}
}