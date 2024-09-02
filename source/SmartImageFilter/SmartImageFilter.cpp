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

inline int getIndex(int startColIndex, int endColIndex, int rowIndex, int colsSize, int maskSize, int maskSizeSquared)
{
	return startColIndex * maskSizeSquared + endColIndex * maskSize + rowIndex;
}

void SmartImageFilter::filter(cv::Mat& newImage, int firstIndex, int lastIndex) {
	auto imageSize = this->data.rows * this->data.cols;
	auto rows = this->data.rows;
	auto cols = this->data.cols;
	//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	auto maskOneHalfLength = this->maskSize / 2;
	const auto maskSizeSquared = std::pow(this->maskSize, 2);
	//Starting value is set in base class in regards to algType
	auto targetValue = this->startingValue;
	//Flatten 3D array
	std::vector<uchar> prefixesPostfixes(cols * maskSize * maskSize);
	auto lastRowIndex = 0;
	auto rowIndex = firstIndex / cols;
	auto colIndex = firstIndex % cols;
	int nextIndex;
	// "i" is always the centre of first mask
	for (auto i = firstIndex; i <= lastIndex; i = nextIndex) {
		//pomyslec nad tym aby w i = std::min(i + maskSize + 1, lastIndex)<-(teraz nextIndex) bylo prawidlowo obliczane
		//Odpowiedz: nie wpadlem na pomysl, aby glowny index obliczac w madrzejzsy sposob, jednym dzialaniem i jednoczesnie obslugiwac dzieki temu indeks kolumn
		//			 wszystko sprowadzalo sie do uzycia ifa, co i tak robie ponizej
		if (colIndex == cols) {
			colIndex = 0;
			++rowIndex;
			i = cols * rowIndex;
		}

		auto firstMaskFarLeftIndex = std::clamp(colIndex - maskOneHalfLength, 0, cols - 1);
		auto firstMaskFarRightIndex = std::clamp(colIndex + maskOneHalfLength, 0, cols - 1);
		auto presecondMaskCentreIndex = std::clamp(colIndex + maskSize, 0, cols - 1);
		auto secondMaskFarLeftIndex = std::clamp(presecondMaskCentreIndex - maskOneHalfLength, 0, cols - 1);
		auto secondMaskFarRightIndex = std::clamp(presecondMaskCentreIndex + maskOneHalfLength, 0, cols - 1);
		auto farTopIndex = std::clamp(rowIndex - maskOneHalfLength, 0, rows - 1);
		auto farBotIndex = std::clamp(rowIndex + maskOneHalfLength, 0, rows - 1);

		auto relativePrefixSuffixRowIndex = 0;
		for (auto j = farTopIndex; j <= farBotIndex; ++j) {
			//pozbyc sie modulo na rzecz ifa (++ z poprzedniej wartosci j)
			prefixesPostfixes[getIndex(firstMaskFarRightIndex, 0, relativePrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)] = this->data.at<uchar>(j, firstMaskFarRightIndex);
			//Prefixes - first mask
			for (auto k = firstMaskFarRightIndex - 1; k >= firstMaskFarLeftIndex; --k) {
				auto currentValue = this->data.at<uchar>(j, k);
				auto oldValue = prefixesPostfixes[getIndex(k + 1, firstMaskFarRightIndex - (k + 1), relativePrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
				if (this->compare(currentValue, oldValue)) {
					prefixesPostfixes[getIndex(k, firstMaskFarRightIndex - k, relativePrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)] = currentValue;
				}
				else {
					prefixesPostfixes[getIndex(k, firstMaskFarRightIndex - k, relativePrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)] = oldValue;
				}
			}

			prefixesPostfixes[getIndex(secondMaskFarLeftIndex, 0, relativePrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)] = this->data.at<uchar>(j, secondMaskFarLeftIndex);
			//Postfixes - secondmask
			for (auto k = secondMaskFarLeftIndex + 1; k <= secondMaskFarRightIndex; ++k) {
				auto currentValue = this->data.at<uchar>(j, k);
				auto oldValue = prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - 1 - secondMaskFarLeftIndex, relativePrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
				if (this->compare(currentValue, oldValue)) {
					prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - secondMaskFarLeftIndex, relativePrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)] = currentValue;
				}
				else {
					prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - secondMaskFarLeftIndex, relativePrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)] = oldValue;
				}
			}
			++relativePrefixSuffixRowIndex;
		}

		auto firstMaskCentreIndex = std::clamp(colIndex, firstMaskFarLeftIndex, firstMaskFarRightIndex);
		auto secondMaskCentreIndex = std::clamp(colIndex + 2 * maskOneHalfLength + 1, secondMaskFarLeftIndex, secondMaskFarRightIndex);

		//Finding best value for left-first index from first mask (it is separate case - we need to only use prefixes from first mask)
		//Iterating from top row of mask up to bottom row of mask
		auto relativeFirstMaskPrefixSuffixRowIndex = 0;
		auto bestValue = prefixesPostfixes[getIndex(firstMaskFarLeftIndex, firstMaskFarRightIndex - firstMaskFarLeftIndex, relativeFirstMaskPrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
		for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
			++relativeFirstMaskPrefixSuffixRowIndex;
			auto currentValue = prefixesPostfixes[getIndex(firstMaskFarLeftIndex, firstMaskFarRightIndex - firstMaskFarLeftIndex, relativeFirstMaskPrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
			if (this->compare(currentValue, bestValue)) {
				bestValue = currentValue;
			}
		}
		newImage.at<uchar>(rowIndex, firstMaskCentreIndex) = bestValue;
		//Finding best value for indices that are mix of prefixes from first mask and postfixes from second mask
		for (auto j = firstMaskCentreIndex + 1; j < secondMaskCentreIndex; ++j) {
			auto currentMaskLeftMost = std::clamp(j - maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
			auto currentMaskRightMost = std::clamp(j + maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
			auto relativeInterMasksPrefixSuffixRowIndex = 0;
			auto firstPartOfMaskValue = prefixesPostfixes[getIndex(currentMaskLeftMost, firstMaskFarRightIndex - currentMaskLeftMost, relativeInterMasksPrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
			auto secondPartOfMaskValue = prefixesPostfixes[getIndex(secondMaskFarLeftIndex, currentMaskRightMost - secondMaskFarLeftIndex, relativeInterMasksPrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
			int bestValue;
			if (this->compare(firstPartOfMaskValue, secondPartOfMaskValue)) {
				bestValue = firstPartOfMaskValue;
			}
			else {
				bestValue = secondPartOfMaskValue;
			}
			//Iterating from top row of mask up to bottom row of mask
			for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
				++relativeInterMasksPrefixSuffixRowIndex;
				firstPartOfMaskValue = prefixesPostfixes[getIndex(currentMaskLeftMost, firstMaskFarRightIndex - currentMaskLeftMost, relativeInterMasksPrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
				secondPartOfMaskValue = prefixesPostfixes[getIndex(secondMaskFarLeftIndex, currentMaskRightMost - secondMaskFarLeftIndex, relativeInterMasksPrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
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
		auto relativeSecondMaskPrefixSuffixRowIndex = 0;
		bestValue = prefixesPostfixes[getIndex(currentMaskLeftMost, currentMaskRightMost - currentMaskLeftMost, relativeSecondMaskPrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
		for (auto k = farTopIndex + 1; k <= farBotIndex; ++k) {
			++relativeSecondMaskPrefixSuffixRowIndex;
			auto currentValue = prefixesPostfixes[getIndex(currentMaskLeftMost, currentMaskRightMost - currentMaskLeftMost, relativeSecondMaskPrefixSuffixRowIndex, cols, maskSize, maskSizeSquared)];
			if (this->compare(currentValue, bestValue)) {
				bestValue = currentValue;
			}
		}
		newImage.at<uchar>(rowIndex, secondMaskCentreIndex) = bestValue;

		if (i == lastIndex)
			break;
		nextIndex = std::min(i + maskSize + 1, lastIndex);
		colIndex += nextIndex;
	}
}