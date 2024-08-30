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

void SmartSignalFilter::apply(std::vector<int>& signal) {
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
	std::vector<int> newSignal(size);
	std::vector<std::thread> threads;
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < threadsNum; i++) {
		int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
		int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
		threads.emplace_back(std::thread(&SmartSignalFilter::filter, this, std::ref(newSignal), firstIndex, lastIndex));
	}
	for (auto& thread : threads)
		thread.join();
	auto end = std::chrono::high_resolution_clock::now();
	const auto execTime = std::chrono::duration<double, std::milli>(end - start).count();
	std::cout << "Smart " << (this->algType == AlgorithmType::MIN ? "min." : "max.") << " signal " << (threadsNum == 1 ? "(single-threaded)" : "(multi-threaded)") << " filter execution time : " << execTime << "ms.\n";
	const auto textFile = this->algType == AlgorithmType::MIN ? (threadsNum == 1 ? minSingleTextFile : minMultiTextFile) : (threadsNum == 1 ? maxSingleTextFile : maxMultiTextFile);
	std::ofstream outfile;
	outfile.open(textFile, std::ios_base::app);
	if (outfile.is_open())
		outfile << execTime << "\n";
	else
		std::cerr << "File: " << textFile << " did not open successfully." << "\n";
	signal = std::move(newSignal);
	outfile.close();
}

inline int getIndex(int startIndex, int endIndex, int maskSize)
{
	//czy na pewno dobrze?
	return startIndex * maskSize + endIndex;
}

void SmartSignalFilter::filter(std::vector<int>& newSignal, int firstIndex, int lastIndex) {
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

/*void SmartSignalFilter::filter(Signal& newPartOfSignal, int firstIndex, int lastIndex) {
	int signalSize = this->data.getSignalSize();
	//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
	int maskOneHalfLength = this->maskSize / 2;
	//Setting the initial value of index of MIN/MAX value that we are looking for to one behind the start of mask
	int indexOfTargetValue = firstIndex - maskOneHalfLength;
	//Starting value is set in base class in regards to algType
	int targetValue = this->startingValue;
	if (partOfSignalType == PartOfSignal::WHOLE) {
		// we need to deal with the cases when the mask does not start at 0 index (it starts before that)

		//By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
		int maskOneHalfLength = this->maskSize / 2;
		//Setting the initial value of index of MIN/MAX value that we are looking for to one behind the start of mask
		int indexOfTargetValue = firstIndex - maskOneHalfLength;
		//Starting value is set in base class in regards to algType
		int targetValue = this->startingValue;

		auto t1 = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < maskOneHalfLength; i++) {
			// we already know that our mask does not start at 0 index
			int leftMostIndexOfMask = 0;
			//Protects us against going out of bounds from right side
			int rightMostIndexOfMask = i + maskOneHalfLength;
			if (rightMostIndexOfMask >= signalSize) {
				rightMostIndexOfMask = signalSize - 1;
			}
			//If index is outside of our window mask, we need to iterate through every number to find the MIN/MAX
			if (indexOfTargetValue < leftMostIndexOfMask || i == firstIndex) {
				targetValue = this->startingValue;
				for (int j = leftMostIndexOfMask; j <= rightMostIndexOfMask; j++) {
					if (this->compare(this->data[j], targetValue)) {
						targetValue = this->data[j];
						indexOfTargetValue = j;
					}
				}
				//If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
				//is more suitable than already found MIN/MAX value
			}
			else {
				if (this->compare(this->data[rightMostIndexOfMask], targetValue)) {
					targetValue = this->data[rightMostIndexOfMask];
					indexOfTargetValue = rightMostIndexOfMask;
				}
			}
			newPartOfSignal->pushBack(targetValue);
			//std::cout << newPartOfSignal->getSignal().back() << " at index: " << i << std::endl;
		}
		for (int startOfPrefixMask = firstIndex; startOfPrefixMask < lastIndex + 1; startOfPrefixMask += maskSize + 1) {
			//case when the start of the prefix mask is the only left index
			if (startOfPrefixMask == signalSize - 1) {
				// we break coz startOfPrefixMask is the only index left (we calculate the next index, which does not exist at this point)
				break;
			}
			int endOfPrefixMask = startOfPrefixMask + this->maskSize - 1;
			int startOfPostfixMask = startOfPrefixMask + maskSize;
			int endOfPostfixMask = startOfPrefixMask + maskSize * 2 - 1;
			//case when the end of the prefix mask is bigger than size
			if (endOfPostfixMask >= signalSize) {
				int indexOfTargetValue = startOfPrefixMask;
				//Starting value is set in base class in regards to algType
				int targetValue = this->startingValue;
				for (int i = startOfPrefixMask + maskOneHalfLength; i < signalSize; i++) {
					int leftMostIndexOfMask = i - maskOneHalfLength;
					//Protects us against going out of bounds from left side
					if (leftMostIndexOfMask < 0) {
						leftMostIndexOfMask = 0;
					}
					//Protects us against going out of bounds from right side
					int rightMostIndexOfMask = i + maskOneHalfLength;
					if (rightMostIndexOfMask >= signalSize) {
						rightMostIndexOfMask = signalSize - 1;
					}
					//If index is outside of our window mask, we need to iterate through every number to find the MIN/MAX
					if (indexOfTargetValue < leftMostIndexOfMask || i == startOfPrefixMask + maskOneHalfLength) {
						targetValue = this->startingValue;
						for (int j = leftMostIndexOfMask; j <= rightMostIndexOfMask; j++) {
							if (this->compare(this->data[j], targetValue)) {
								targetValue = this->data[j];
								indexOfTargetValue = j;
							}
						}
						//If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
						//is more suitable than already found MIN/MAX value
					}
					else {
						if (this->compare(this->data[rightMostIndexOfMask], targetValue)) {
							targetValue = this->data[rightMostIndexOfMask];
							indexOfTargetValue = rightMostIndexOfMask;
						}
					}
					newPartOfSignal->pushBack(targetValue);
				}
				break;
			}
			this->prePostFixes[endOfPrefixMask][endOfPrefixMask] = this->data[endOfPrefixMask];
			int lastBeforeLastPrefixMaskIndex = startOfPrefixMask + this->maskSize - 2;
			if (lastBeforeLastPrefixMaskIndex >= signalSize) {
				lastBeforeLastPrefixMaskIndex = signalSize - 2;
			}
			for (int j = lastBeforeLastPrefixMaskIndex; j >= startOfPrefixMask; j--) {
				if (this->compare(this->data[j], this->prePostFixes[j + 1][endOfPrefixMask])) {
					this->prePostFixes[j][endOfPrefixMask] = this->data[j];
				}
				else {
					this->prePostFixes[j][endOfPrefixMask] = this->prePostFixes[j + 1][endOfPrefixMask];
				}
			}
			this->prePostFixes[startOfPostfixMask][startOfPostfixMask] = this->data[startOfPostfixMask];
			int nextAfterStartIndexOfPostfixMask = startOfPostfixMask + 1;
			if (nextAfterStartIndexOfPostfixMask >= signalSize) {
				nextAfterStartIndexOfPostfixMask = signalSize - 1;
			}
			if (nextAfterStartIndexOfPostfixMask < signalSize) {
				for (int j = nextAfterStartIndexOfPostfixMask; j < startOfPostfixMask + this->maskSize; j++) {
					if (this->compare(this->data[j], this->prePostFixes[startOfPostfixMask][j - 1])) {
						this->prePostFixes[startOfPostfixMask][j] = this->data[j];
					}
					else {
						this->prePostFixes[startOfPostfixMask][j] = this->prePostFixes[startOfPostfixMask][j - 1];
					}
				}
			}

			//
			for (int j = startOfPrefixMask; j <= startOfPostfixMask; j++) {
				//it is already checked by this previous for loop case when the mask does not start at index 0
				int leftMostIndexOfMask = j;

				//I think that might not be needed since we restrict it in j loop values
				int rightMostIndexOfMask = leftMostIndexOfMask + maskOneHalfLength * 2;
				if (rightMostIndexOfMask > endOfPostfixMask) {
					rightMostIndexOfMask = endOfPostfixMask;
				}

				//if right index is in postfix part, we need to properly use prefix and postfix part
				if (j == startOfPostfixMask) {
					newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask]);
				}
				else if (rightMostIndexOfMask >= startOfPostfixMask) {
					if (this->compare(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask], this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask])) {
						newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask]);
					}
					else {
						newPartOfSignal->pushBack(this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask]);
					}
				}
				else {
					newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask]);
				}
			}
		}
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> ms_double = t2 - t1;
		std::cout << ms_double.count() << "ms\n";
	}
	else if (partOfSignalType == PartOfSignal::START) {
		for (int i = 0; i < maskOneHalfLength; i++) {
			// we already know that our mask does not start at 0 index
			int leftMostIndexOfMask = 0;
			//Protects us against going out of bounds from right side
			int rightMostIndexOfMask = i + maskOneHalfLength;
			if (rightMostIndexOfMask >= signalSize) {
				rightMostIndexOfMask = signalSize - 1;
			}
			//If index is outside of our window mask, we need to iterate through every number to find the MIN/MAX
			if (indexOfTargetValue < leftMostIndexOfMask || i == firstIndex) {
				targetValue = this->startingValue;
				for (int j = leftMostIndexOfMask; j <= rightMostIndexOfMask; j++) {
					if (this->compare(this->data[j], targetValue)) {
						targetValue = this->data[j];
						indexOfTargetValue = j;
					}
				}
				//If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
				//is more suitable than already found MIN/MAX value
			}
			else {
				if (this->compare(this->data[rightMostIndexOfMask], targetValue)) {
					targetValue = this->data[rightMostIndexOfMask];
					indexOfTargetValue = rightMostIndexOfMask;
				}
			}
			newPartOfSignal->pushBack(targetValue);
			std::cout << newPartOfSignal->getSignal().back() << " at index: " << i << std::endl;
		}

		for (int startOfPrefixMask = firstIndex; startOfPrefixMask < lastIndex + 1; startOfPrefixMask += maskSize + 1) {
			if (startOfPrefixMask == signalSize - 1) {
				// we break coz startOfPrefixMask is the only index left (we calculate the next index, which does not exist at this point)
				return;
			}
			int endOfPrefixMask = startOfPrefixMask + this->maskSize - 1;
			int startOfPostfixMask = startOfPrefixMask + maskSize;
			int endOfPostfixMask = startOfPrefixMask + maskSize * 2 - 1;
			//case when the end of the prefix mask is bigger than size
			if (endOfPostfixMask >= signalSize) {
				int indexOfTargetValue = startOfPrefixMask;
				//Starting value is set in base class in regards to algType
				int targetValue = this->startingValue;
				for (int i = startOfPrefixMask + maskOneHalfLength; i < signalSize; i++) {
					int leftMostIndexOfMask = i - maskOneHalfLength;
					//Protects us against going out of bounds from left side
					if (leftMostIndexOfMask < 0) {
						leftMostIndexOfMask = 0;
					}
					//Protects us against going out of bounds from right side
					int rightMostIndexOfMask = i + maskOneHalfLength;
					if (rightMostIndexOfMask >= signalSize) {
						rightMostIndexOfMask = signalSize - 1;
					}
					//If index is outside of our window mask, we need to iterate through every number to find the MIN/MAX
					if (indexOfTargetValue < leftMostIndexOfMask || i == startOfPrefixMask + maskOneHalfLength) {
						targetValue = this->startingValue;
						for (int j = leftMostIndexOfMask; j <= rightMostIndexOfMask; j++) {
							if (this->compare(this->data[j], targetValue)) {
								targetValue = this->data[j];
								indexOfTargetValue = j;
							}
						}
						//If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
						//is more suitable than already found MIN/MAX value
					}
					else {
						if (this->compare(this->data[rightMostIndexOfMask], targetValue)) {
							targetValue = this->data[rightMostIndexOfMask];
							indexOfTargetValue = rightMostIndexOfMask;
						}
					}
					newPartOfSignal->pushBack(targetValue);
					std::cout << newPartOfSignal->getSignal().back() << " at index: " << i << std::endl;
				}
				return;
				// it means that we can calculate the indexes(by that i mean the middle indexes of masks) up to
				// middle of prefix(postfix really) mask and up to some index which is defined by the last index of
				// endOfPostfixMask (not directly, it helps define it in some way)
				// thus, the approach to calc the indexes after the middle one of prefix(postfix really) mask needs to be implemented
				// it can be done in a way that we use smart approach as long as we can and then switch to naive
			}
			this->prePostFixes[endOfPrefixMask][endOfPrefixMask] = this->data[endOfPrefixMask];
			int lastBeforeLastPrefixMaskIndex = startOfPrefixMask + this->maskSize - 2; //coz we check j + 1
			if (lastBeforeLastPrefixMaskIndex >= signalSize) {
				lastBeforeLastPrefixMaskIndex = signalSize - 2;
			}
			for (int j = lastBeforeLastPrefixMaskIndex; j >= startOfPrefixMask; j--) {
				if (this->compare(this->data[j], this->prePostFixes[j + 1][endOfPrefixMask])) {
					this->prePostFixes[j][endOfPrefixMask] = this->data[j];
				}
				else {
					this->prePostFixes[j][endOfPrefixMask] = this->prePostFixes[j + 1][endOfPrefixMask];
				}
			}
			this->prePostFixes[startOfPostfixMask][startOfPostfixMask] = this->data[startOfPostfixMask];
			int nextAfterStartIndexOfPostfixMask = startOfPostfixMask + 1;
			if (nextAfterStartIndexOfPostfixMask >= signalSize) {
				nextAfterStartIndexOfPostfixMask = signalSize - 1;
			}
			if (nextAfterStartIndexOfPostfixMask < signalSize) {
				for (int j = nextAfterStartIndexOfPostfixMask; j < startOfPostfixMask + this->maskSize; j++) {
					if (this->compare(this->data[j], this->prePostFixes[startOfPostfixMask][j - 1])) {
						this->prePostFixes[startOfPostfixMask][j] = this->data[j];
					}
					else {
						this->prePostFixes[startOfPostfixMask][j] = this->prePostFixes[startOfPostfixMask][j - 1];
					}
				}
			}

			for (int j = startOfPrefixMask; j <= startOfPostfixMask; j++) {
				//it is already checked by this previous for loop case when the mask does not start at index 0
				int leftMostIndexOfMask = j;

				//I think that might not be needed since we restrict it in j loop values
				int rightMostIndexOfMask = leftMostIndexOfMask + maskOneHalfLength * 2;
				if (rightMostIndexOfMask > endOfPostfixMask) {
					rightMostIndexOfMask = endOfPostfixMask;
				}

				//if right index is in postfix part, we need to properly use prefix and postfix part
				if (j == startOfPostfixMask) {
					newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask]);
				}
				else if (rightMostIndexOfMask >= startOfPostfixMask) {
					if (this->compare(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask], this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask])) {
						newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask]);
					}
					else {
						newPartOfSignal->pushBack(this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask]);
					}
				}
				else {
					newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask]);
				}
				std::cout << newPartOfSignal->getSignal().back() << " at index: " << j + maskOneHalfLength << std::endl;
			}
		}
	}
	else if (partOfSignalType == PartOfSignal::MID) {

		for (int startOfPrefixMask = firstIndex; startOfPrefixMask < lastIndex + 1; startOfPrefixMask += maskSize + 1) {
			if (startOfPrefixMask == signalSize - 1) {
				// we break coz startOfPrefixMask is the only index left (we calculate the next index, which does not exist at this point)
				return;
			}
			int endOfPrefixMask = startOfPrefixMask + this->maskSize - 1;
			int startOfPostfixMask = startOfPrefixMask + maskSize;
			int endOfPostfixMask = startOfPrefixMask + maskSize * 2 - 1;
			//case when the end of the prefix mask is bigger than size
			if (endOfPostfixMask >= signalSize) {
				int indexOfTargetValue = startOfPrefixMask;
				//Starting value is set in base class in regards to algType
				int targetValue = this->startingValue;
				// we add maskOneHalfLength, because if it occurs in MID part, the next, which is END part, assumes that it is calculate up to that point
				for (int i = startOfPrefixMask + maskOneHalfLength; i <= lastIndex + maskOneHalfLength; i++) {
					int leftMostIndexOfMask = i - maskOneHalfLength;
					//Protects us against going out of bounds from left side
					if (leftMostIndexOfMask < 0) {
						leftMostIndexOfMask = 0;
					}
					//Protects us against going out of bounds from right side
					int rightMostIndexOfMask = i + maskOneHalfLength;
					if (rightMostIndexOfMask >= signalSize) {
						rightMostIndexOfMask = signalSize - 1;
					}
					//If index is outside of our window mask, we need to iterate through every number to find the MIN/MAX
					if (indexOfTargetValue < leftMostIndexOfMask || i == startOfPrefixMask + maskOneHalfLength) {
						targetValue = this->startingValue;
						for (int j = leftMostIndexOfMask; j <= rightMostIndexOfMask; j++) {
							if (this->compare(this->data[j], targetValue)) {
								targetValue = this->data[j];
								indexOfTargetValue = j;
							}
						}
						//If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
						//is more suitable than already found MIN/MAX value
					}
					else {
						if (this->compare(this->data[rightMostIndexOfMask], targetValue)) {
							targetValue = this->data[rightMostIndexOfMask];
							indexOfTargetValue = rightMostIndexOfMask;
						}
					}
					newPartOfSignal->pushBack(targetValue);
					std::cout << newPartOfSignal->getSignal().back() << " at index: " << i << std::endl;
				}
				return;
				// it means that we can calculate the indexes(by that i mean the middle indexes of masks) up to
				// middle of prefix(postfix really) mask and up to some index which is defined by the last index of
				// endOfPostfixMask (not directly, it helps define it in some way)
				// thus, the approach to calc the indexes after the middle one of prefix(postfix really) mask needs to be implemented
				// it can be done in a way that we use smart approach as long as we can and then switch to naive
			}
			this->prePostFixes[endOfPrefixMask][endOfPrefixMask] = this->data[endOfPrefixMask];
			int lastBeforeLastPrefixMaskIndex = startOfPrefixMask + this->maskSize - 2; //coz we check j + 1
			if (lastBeforeLastPrefixMaskIndex >= signalSize) {
				lastBeforeLastPrefixMaskIndex = signalSize - 2;
			}
			for (int j = lastBeforeLastPrefixMaskIndex; j >= startOfPrefixMask; j--) {
				if (this->compare(this->data[j], this->prePostFixes[j + 1][endOfPrefixMask])) {
					this->prePostFixes[j][endOfPrefixMask] = this->data[j];
				}
				else {
					this->prePostFixes[j][endOfPrefixMask] = this->prePostFixes[j + 1][endOfPrefixMask];
				}
			}
			this->prePostFixes[startOfPostfixMask][startOfPostfixMask] = this->data[startOfPostfixMask];
			int nextAfterStartIndexOfPostfixMask = startOfPostfixMask + 1;
			if (nextAfterStartIndexOfPostfixMask >= signalSize) {
				nextAfterStartIndexOfPostfixMask = signalSize - 1;
			}
			if (nextAfterStartIndexOfPostfixMask < signalSize) {
				for (int j = nextAfterStartIndexOfPostfixMask; j < startOfPostfixMask + this->maskSize; j++) {
					if (this->compare(this->data[j], this->prePostFixes[startOfPostfixMask][j - 1])) {
						this->prePostFixes[startOfPostfixMask][j] = this->data[j];
					}
					else {
						this->prePostFixes[startOfPostfixMask][j] = this->prePostFixes[startOfPostfixMask][j - 1];
					}
				}
			}

			//
			for (int j = startOfPrefixMask; j <= startOfPostfixMask; j++) {
				//it is already checked by this previous for loop case when the mask does not start at index 0
				int leftMostIndexOfMask = j;

				//I think that might not be needed since we restrict it in j loop values
				int rightMostIndexOfMask = leftMostIndexOfMask + maskOneHalfLength * 2;
				if (rightMostIndexOfMask > endOfPostfixMask) {
					rightMostIndexOfMask = endOfPostfixMask;
				}

				//if right index is in postfix part, we need to properly use prefix and postfix part
				if (j == startOfPostfixMask) {
					if (this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask] == 0) {
						std::cout << "zero" << std::endl;
					}
					newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask]);
				}
				else if (rightMostIndexOfMask >= startOfPostfixMask) {
					if (this->compare(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask], this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask])) {
						if (this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask] == 0) {
							std::cout << "zero" << std::endl;
						}
						newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask]);
					}
					else {
						if (this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask] == 0) {
							std::cout << "zero" << std::endl;
						}
						newPartOfSignal->pushBack(this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask]);
					}
				}
				else {
					if (this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask] == 0) {
						std::cout << "zero" << std::endl;
					}
					newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask]);
				}
				std::cout << newPartOfSignal->getSignal().back() << " at index: " << j + maskOneHalfLength << std::endl;
			}
		}
	}
	else if (partOfSignalType == PartOfSignal::END) {
		for (int startOfPrefixMask = firstIndex; startOfPrefixMask < lastIndex + 1; startOfPrefixMask += maskSize + 1) {
			if (startOfPrefixMask == signalSize - 1) {
				// we break coz startOfPrefixMask is the only index left (we calculate the next index, which does not exist at this point)
				return;
			}
			int endOfPrefixMask = startOfPrefixMask + this->maskSize - 1;
			int startOfPostfixMask = startOfPrefixMask + maskSize;
			int endOfPostfixMask = startOfPrefixMask + maskSize * 2 - 1;
			//case when the end of the prefix mask is bigger than size
			if (endOfPostfixMask >= signalSize) {
				int indexOfTargetValue = startOfPrefixMask;
				//Starting value is set in base class in regards to algType
				int targetValue = this->startingValue;
				for (int i = startOfPrefixMask + maskOneHalfLength; i < signalSize; i++) {
					int leftMostIndexOfMask = i - maskOneHalfLength;
					//Protects us against going out of bounds from left side
					if (leftMostIndexOfMask < 0) {
						leftMostIndexOfMask = 0;
					}
					//Protects us against going out of bounds from right side
					int rightMostIndexOfMask = i + maskOneHalfLength;
					if (rightMostIndexOfMask >= signalSize) {
						rightMostIndexOfMask = signalSize - 1;
					}
					//If index is outside of our window mask, we need to iterate through every number to find the MIN/MAX
					if (indexOfTargetValue < leftMostIndexOfMask || i == startOfPrefixMask + maskOneHalfLength) {
						targetValue = this->startingValue;
						for (int j = leftMostIndexOfMask; j <= rightMostIndexOfMask; j++) {
							if (this->compare(this->data[j], targetValue)) {
								targetValue = this->data[j];
								indexOfTargetValue = j;
							}
						}
						//If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
						//is more suitable than already found MIN/MAX value
					}
					else {
						if (this->compare(this->data[rightMostIndexOfMask], targetValue)) {
							targetValue = this->data[rightMostIndexOfMask];
							indexOfTargetValue = rightMostIndexOfMask;
						}
					}
					newPartOfSignal->pushBack(targetValue);
					std::cout << newPartOfSignal->getSignal().back() << " at index: " << i << std::endl;
				}
				return;
				// it means that we can calculate the indexes(by that i mean the middle indexes of masks) up to
				// middle of prefix(postfix really) mask and up to some index which is defined by the last index of
				// endOfPostfixMask (not directly, it helps define it in some way)
				// thus, the approach to calc the indexes after the middle one of prefix(postfix really) mask needs to be implemented
				// it can be done in a way that we use smart approach as long as we can and then switch to naive
			}
			this->prePostFixes[endOfPrefixMask][endOfPrefixMask] = this->data[endOfPrefixMask];
			int lastBeforeLastPrefixMaskIndex = startOfPrefixMask + this->maskSize - 2; //coz we check j + 1
			if (lastBeforeLastPrefixMaskIndex >= signalSize) {
				lastBeforeLastPrefixMaskIndex = signalSize - 2;
			}
			for (int j = lastBeforeLastPrefixMaskIndex; j >= startOfPrefixMask; j--) {
				if (this->compare(this->data[j], this->prePostFixes[j + 1][endOfPrefixMask])) {
					this->prePostFixes[j][endOfPrefixMask] = this->data[j];
				}
				else {
					this->prePostFixes[j][endOfPrefixMask] = this->prePostFixes[j + 1][endOfPrefixMask];
				}
			}
			this->prePostFixes[startOfPostfixMask][startOfPostfixMask] = this->data[startOfPostfixMask];
			int nextAfterStartIndexOfPostfixMask = startOfPostfixMask + 1;
			if (nextAfterStartIndexOfPostfixMask >= signalSize) {
				nextAfterStartIndexOfPostfixMask = signalSize - 1;
			}
			if (nextAfterStartIndexOfPostfixMask < signalSize) {
				for (int j = nextAfterStartIndexOfPostfixMask; j < startOfPostfixMask + this->maskSize; j++) {
					if (this->compare(this->data[j], this->prePostFixes[startOfPostfixMask][j - 1])) {
						this->prePostFixes[startOfPostfixMask][j] = this->data[j];
					}
					else {
						this->prePostFixes[startOfPostfixMask][j] = this->prePostFixes[startOfPostfixMask][j - 1];
					}
				}
			}

			//
			for (int j = startOfPrefixMask; j <= startOfPostfixMask; j++) {
				//it is already checked by this previous for loop case when the mask does not start at index 0
				int leftMostIndexOfMask = j;

				//I think that might not be needed since we restrict it in j loop values
				int rightMostIndexOfMask = leftMostIndexOfMask + maskOneHalfLength * 2;
				if (rightMostIndexOfMask > endOfPostfixMask) {
					rightMostIndexOfMask = endOfPostfixMask;
				}

				//if right index is in postfix part, we need to properly use prefix and postfix part
				if (j == startOfPostfixMask) {
					newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask]);
				}
				else if (rightMostIndexOfMask >= startOfPostfixMask) {
					if (this->compare(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask], this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask])) {
						newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask]);
					}
					else {
						newPartOfSignal->pushBack(this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask]);
					}
				}
				else {
					newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][rightMostIndexOfMask]);
				}
				std::cout << newPartOfSignal->getSignal().back() << " at index: " << j + maskOneHalfLength << std::endl;
			}
		}
	}
}*/