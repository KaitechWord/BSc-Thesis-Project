#include "SmartSignalFilter.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <thread>
#include <fstream>

static const auto minSingleTextFile = std::string(ROOT_DIR) + "/output/SignalSmartMinSingle.txt";
static const auto minMultiTextFile = std::string(ROOT_DIR) + "/output/SignalSmartMinMulti.txt";
static const auto maxSingleTextFile = std::string(ROOT_DIR) + "/output/SignalSmartMaxSingle.txt";
static const auto maxMultiTextFile = std::string(ROOT_DIR) + "/output/SignalSmartMaxMulti.txt";

SmartSignalFilter::SmartSignalFilter(int threadNum, AlgorithmType algType, int maskSize)
	: SignalFilter(threadNum, algType, maskSize)
{}

void SmartSignalFilter::apply(std::vector<uint8_t>& signal) {
	this->data = signal;
	int size = signal.size();
	int threadsNum = this->threadNum;
	if (size < maskSize * 2) {
		std::cout << "Too small signal size. It is smaller than size of two masks. Applying smart filter would not differ from using naive approach\n";
		return;
	}
	if (threadsNum > size) {
		std::cout << "The number of threads is bigger than the size of the signal. Setting number of threads to size.\n";
		threadsNum = size;
	}
	int sizeOfOneThread = size / threadsNum;
	int remainder = size % threadsNum;
	std::vector<std::vector<uint8_t>> newSignals(threadsNum, std::vector<uint8_t>(size));
	std::vector<std::thread> threads;
	std::cout << "Number of elements: " << this->data.size() << std::endl;
	std::cout << "MaskSize: " << this->maskSize << std::endl;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < threadsNum; i++) {
		int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		threads.emplace_back(std::thread(&SmartSignalFilter::filter, this, std::ref(newSignals[i]), firstIndex, lastIndex));
	}
	for (auto& thread : threads)
		thread.join();
	auto end = std::chrono::high_resolution_clock::now();
	const auto execTime = std::chrono::duration<double, std::milli>(end - start).count();
	std::cout << "Smart " << (this->algType == AlgorithmType::MIN ? "min." : "max.") << " signal " << (threadsNum == 1 ? "(single-threaded)" : "(multi-threaded)") << " filter execution time : " << execTime << "ms.\n";
	std::cout << std::fixed << "Elem/sec:" << this->data.size() / (execTime * 0.001) << std::endl;
	const auto textFile = this->algType == AlgorithmType::MIN ? (threadsNum == 1 ? minSingleTextFile : minMultiTextFile) : (threadsNum == 1 ? maxSingleTextFile : maxMultiTextFile);
	std::ofstream outfile;
	outfile.open(textFile, std::ios_base::app);
	if (outfile.is_open())
		outfile << execTime << "\n";
	else
		std::cerr << "File: " << textFile << " did not open successfully." << "\n";
	for (int i = 0; i < threadsNum; ++i) {
		int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		for (int j = firstIndex; j <= lastIndex; ++j) {
			signal[j] = newSignals[i][j];
		}
	}
	outfile.close();
}

inline int getIndex(int startIndex, int endIndex, int maskSize)
{
	return startIndex * maskSize + endIndex;
}

void SmartSignalFilter::filter(std::vector<uint8_t>& newSignal, int firstIndex, int lastIndex) {
	auto signalSize = static_cast<int>(this->data.size());
	//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	auto maskOneHalfLength = this->maskSize / 2;
	//Starting value is set in base class in regards to algType
	auto targetValue = this->startingValue;
	//Flatten 3D array
	std::vector<int> prefixesPostfixes(signalSize * this->maskSize);
	auto lastRowIndex = 0;
	// "i" is always the centre of first mask
	for (auto i = firstIndex; i <= lastIndex; i = std::min(i + this->maskSize + 1, lastIndex)) {
		auto firstMaskFarLeftIndex = std::clamp(i - maskOneHalfLength, 0, signalSize - 1);
		auto firstMaskFarRightIndex = std::clamp(i + maskOneHalfLength, 0, signalSize - 1);
		auto presecondMaskCentreIndex = std::clamp(i + maskSize, 0, signalSize - 1);
		auto secondMaskFarLeftIndex = std::clamp(presecondMaskCentreIndex - maskOneHalfLength, 0, signalSize - 1);
		auto secondMaskFarRightIndex = std::clamp(presecondMaskCentreIndex + maskOneHalfLength, 0, signalSize - 1);

		prefixesPostfixes[getIndex(firstMaskFarRightIndex, 0, maskSize)] = this->data[firstMaskFarRightIndex];
		//Prefixes - first mask
		for (auto k = firstMaskFarRightIndex - 1; k >= firstMaskFarLeftIndex; --k) {
			auto currentValue = this->data[k];
			auto oldValue = prefixesPostfixes[getIndex(k + 1, firstMaskFarRightIndex - (k + 1), maskSize)];
			if (this->compare(currentValue, oldValue)) {
				prefixesPostfixes[getIndex(k, firstMaskFarRightIndex - k, maskSize)] = currentValue;
			}
			else {
				prefixesPostfixes[getIndex(k, firstMaskFarRightIndex - k, maskSize)] = oldValue;
			}
		}

		prefixesPostfixes[getIndex(secondMaskFarLeftIndex, 0, maskSize)] = this->data[secondMaskFarLeftIndex];
		//Postfixes - secondmask
		for (auto k = secondMaskFarLeftIndex + 1; k <= secondMaskFarRightIndex; ++k) {
			auto currentValue = this->data[k];
			auto oldValue = prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - 1 - secondMaskFarLeftIndex, maskSize)];
			if (this->compare(currentValue, oldValue)) {
				prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - secondMaskFarLeftIndex, maskSize)] = currentValue;
			}
			else {
				prefixesPostfixes[getIndex(secondMaskFarLeftIndex, k - secondMaskFarLeftIndex, maskSize)] = oldValue;
			}
		}

		auto firstMaskCentreIndex = std::clamp(i, firstMaskFarLeftIndex, firstMaskFarRightIndex);
		auto secondMaskCentreIndex = std::clamp(i + 2 * maskOneHalfLength + 1, secondMaskFarLeftIndex, secondMaskFarRightIndex);

		//Finding best value for left-first index from first mask (it is separate case - we need to only use prefixes from first mask)
		auto bestValue = prefixesPostfixes[getIndex(firstMaskFarLeftIndex, firstMaskFarRightIndex - firstMaskFarLeftIndex, maskSize)];
		newSignal[firstMaskCentreIndex] = bestValue;
		//Finding best value for indices that are mix of prefixes from first mask and postfixes from second mask
		for (auto j = firstMaskCentreIndex + 1; j < secondMaskCentreIndex; ++j) {
			auto currentMaskLeftMost = std::clamp(j - maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
			auto currentMaskRightMost = std::clamp(j + maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
			auto firstPartOfMaskValue = prefixesPostfixes[getIndex(currentMaskLeftMost, firstMaskFarRightIndex - currentMaskLeftMost, maskSize)];
			auto secondPartOfMaskValue = prefixesPostfixes[getIndex(secondMaskFarLeftIndex, currentMaskRightMost - secondMaskFarLeftIndex, maskSize)];
			int bestValue;
			if (this->compare(firstPartOfMaskValue, secondPartOfMaskValue)) {
				bestValue = firstPartOfMaskValue;
			}
			else {
				bestValue = secondPartOfMaskValue;
			}
			newSignal[j] = bestValue;
		}
		auto currentMaskLeftMost = std::clamp(secondMaskCentreIndex - maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
		auto currentMaskRightMost = std::clamp(secondMaskCentreIndex + maskOneHalfLength, firstMaskFarLeftIndex, secondMaskFarRightIndex);
		//Finding best value for last index from second mask (it is separate case - we need to only use postfixes from second mask)
		newSignal[secondMaskCentreIndex] = prefixesPostfixes[getIndex(currentMaskLeftMost, currentMaskRightMost - currentMaskLeftMost, maskSize)];

		if (i == lastIndex)
			break;
	}
}