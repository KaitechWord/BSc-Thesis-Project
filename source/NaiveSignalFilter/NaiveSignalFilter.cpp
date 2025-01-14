#include "NaiveSignalFilter.h"
#include <iostream>
#include <chrono>
#include <algorithm>
#include <thread>
#include <fstream>
#include <random>

static const auto minSingleTextFile = std::string(ROOT_DIR) + "/output/SignalNaiveMinSingle.txt";
static const auto minMultiTextFile = std::string(ROOT_DIR) + "/output/SignalNaiveMinMulti.txt";
static const auto maxSingleTextFile = std::string(ROOT_DIR) + "/output/SignalNaiveMaxSingle.txt";
static const auto maxMultiTextFile = std::string(ROOT_DIR) + "/output/SignalNaiveMaxMulti.txt";

NaiveSignalFilter::NaiveSignalFilter(int threadNum, AlgorithmType algType, int maskSize)
	: SignalFilter(threadNum, algType, maskSize)
{}

void NaiveSignalFilter::apply(std::vector<uint8_t>& signal) {
	this->data = signal;
	int size = signal.size();
	int threadsNum = this->threadNum;
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
	for (int i = 0; i < threadsNum; ++i) {
		int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		threads.emplace_back(std::thread(&NaiveSignalFilter::filter, this, std::ref(newSignals[i]), firstIndex, lastIndex));
	}
	for (auto& thread : threads)
		thread.join();
	auto end = std::chrono::high_resolution_clock::now();
	const auto execTime = std::chrono::duration<double, std::milli>(end - start).count();
	std::cout << "Naive " << (this->algType == AlgorithmType::MIN ? "min." : "max.") << " signal " << (threadsNum == 1 ? "(single-threaded)" : "(multi-threaded)") << " filter execution time : " << execTime << "ms.\n";
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

void NaiveSignalFilter::filter(std::vector<uint8_t>& newSignal, int firstIndex, int lastIndex) {
	int signalSize = this->data.size();
	//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	int maskOneHalfLength = this->maskSize / 2;
	//Setting the initial value of index of MIN/MAX value that we are looking for to one 	behind the start of mask
	int indexOfTargetValue = std::clamp(firstIndex - maskOneHalfLength, 0, signalSize - 1);
	//Starting value is set in base class in regards to algType
	auto targetValue = this->startingValue;
	if (this->algType == AlgorithmType::MIN) {
		if( firstIndex < maskOneHalfLength && firstIndex + maskOneHalfLength > this->data.size ) {
			
		}
		else if( firstIndex < maskOneHalfLength ) {
			for (int i = firstIndex; i <= maskOneHalfLength; ++i) {
			int leftMostIndexOfMask = std::clamp(i - maskOneHalfLength, 0, signalSize - 1);
			int rightMostIndexOfMask = std::clamp(i + maskOneHalfLength, 0, signalSize - 1);
			newSignal[i] = *std::min_element(this->data.cbegin() + leftMostIndexOfMask, this->data.cbegin() + rightMostIndexOfMask + 1);
		}
		}
		for (int i = firstIndex; i <= lastIndex; ++i) {
			int leftMostIndexOfMask = std::clamp(i - maskOneHalfLength, 0, signalSize - 1);
			int rightMostIndexOfMask = std::clamp(i + maskOneHalfLength, 0, signalSize - 1);
			newSignal[i] = *std::min_element(this->data.cbegin() + leftMostIndexOfMask, this->data.cbegin() + rightMostIndexOfMask + 1);
		}
	}
	else {
		for (int i = firstIndex; i <= lastIndex; ++i) {
			int leftMostIndexOfMask = std::clamp(i - maskOneHalfLength, 0, signalSize - 1);
			int rightMostIndexOfMask = std::clamp(i + maskOneHalfLength, 0, signalSize - 1);
			newSignal[i] = *std::max_element(this->data.cbegin() + leftMostIndexOfMask, this->data.cbegin() + rightMostIndexOfMask + 1);
		}
	}

}