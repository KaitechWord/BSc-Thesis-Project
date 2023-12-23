#include "SmartSignalFilter.h"
#include <iostream>
#include <map>

SmartSignalFilter::SmartSignalFilter(int threadNum, AlgorithmType algType, int maskSize)
    : SignalFilter(threadNum, algType, maskSize)
{}

void SmartSignalFilter::apply(Signal& signal) {
    this->data = signal;
    int size = signal.getSignalSize();
    int threadsNum = this->tp.getThreadsNum();
    if (threadsNum > size) {
        std::cout << "The number of threads is bigger than the size of the signal. Setting number of threads to size.\n";
        threadsNum = size;
    }
    int sizeOfOneThread = size / threadsNum;
    int remainder = size % threadsNum;
    std::vector<std::shared_ptr<Signal>> partsOfSignal;
    this->tp.start();
    std::map<int, std::map<int, int>> prePostFixes;
    std::mutex prePostFixesL;
    std::shared_ptr<std::atomic<bool>>  isPreviousDoneWithEndPart = nullptr;
    std::shared_ptr<std::atomic<bool>>  amIDoneWithEndPart = nullptr;
    for (int i = 0; i < threadsNum; i++) {
        PartOfSignal partOfSignalType = PartOfSignal::MID;
        int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
        int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
        partsOfSignal.emplace_back(std::make_shared<Signal>());
        std::shared_ptr<Signal> partOfSignal = partsOfSignal.back();
        if(firstIndex == 0 && lastIndex == size){
            partOfSignalType = PartOfSignal::WHOLE;
        } else if (firstIndex == 0) {
            partOfSignalType = PartOfSignal::START;
            amIDoneWithEndPart = std::make_shared<std::atomic<bool>>();
        } else if (lastIndex == size) {
            partOfSignalType = PartOfSignal::END;
            isPreviousDoneWithEndPart = amIDoneWithEndPart;
            amIDoneWithEndPart = nullptr;
        } else { //MID
            isPreviousDoneWithEndPart = amIDoneWithEndPart;
            amIDoneWithEndPart = std::make_shared<std::atomic<bool>>();
        }
        this->tp.queueJob([this, partOfSignal, firstIndex, lastIndex]() { this->filter(partOfSignal, firstIndex, lastIndex, isPreviousDoneWithEndPart, amIDoneWithEndPart); });
    }
    while (this->tp.busy()) {};
    this->tp.stop();
    for (int i = 0; i < threadsNum; i++) {
        int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
        int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
        int partOfSignalIndex = 0;
        for (int j = firstIndex; j <= lastIndex; j++) {
            signal[j] = partsOfSignal[i]->getSignal()[partOfSignalIndex++];
        }
    }
}

// need to remember that the map is not thread safe, even though i assign values to different keys
// sauce: https://stackoverflow.com/questions/62105665/c-multithreading-writing-to-different-keys-in-same-map-causing-problems
void SmartSignalFilter::filter(const Signal& oldSignal, std::shared_ptr<Signal> newPartOfSignal, int firstIndex, int lastIndex, PartOfSignal partOfSignal, std::shared_ptr<std::atomic<bool>> isPreviousDoneWithEndPart, std::shared_ptr<std::atomic<bool>> amIDoneWithEndPart) {
    int signalSize = oldSignal.getSignalSize();
    //By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
    int maskOneHalfLength = this->maskSize / 2;
    //Setting the initial value of index of MIN/MAX value that we are looking for to one behind the start of mask
    int indexOfTargetValue = firstIndex - maskOneHalfLength;
    //Starting value is set in base class in regards to algType
    int targetValue = this->startingValue;
    for (int i = firstIndex; i <= lastIndex; i++) {
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
        
        newPartOfSignal->pushBack(targetValue);
    }
}