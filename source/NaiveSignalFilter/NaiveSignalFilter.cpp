#include "NaiveSignalFilter.h"
#include <iostream>
#include <chrono>

NaiveSignalFilter::NaiveSignalFilter(int threadNum, AlgorithmType algType, int maskSize)
    : SignalFilter(threadNum, algType, maskSize)
{}

void NaiveSignalFilter::apply(Signal& signal) {
    this->data = signal;
    int size = signal.getSignalSize();
    int threadsNum = 1;//this->tp.getThreadsNum();
    if (threadsNum > size) {
        std::cout << "The number of threads is bigger than the size of the signal. Setting number of threads to size.\n";
        threadsNum = size;
    }
    int sizeOfOneThread = size / threadsNum;
    int remainder = size % threadsNum;
    std::vector<std::shared_ptr<Signal>> partsOfSignal;

    this->tp.start();
    for (int i = 0; i < threadsNum; i++) {
        int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
        int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
        partsOfSignal.emplace_back(std::make_shared<Signal>());
        std::shared_ptr<Signal> partOfSignal = partsOfSignal.back();
        this->tp.queueJob([this, partOfSignal, firstIndex, lastIndex]() { this->filter(partOfSignal, firstIndex, lastIndex); });
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

void NaiveSignalFilter::filter(std::shared_ptr<Signal> newPartOfSignal, int firstIndex, int lastIndex){
    int signalSize = this->data.getSignalSize();
    //By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
    int maskOneHalfLength = this->maskSize / 2;
    //Setting the initial value of index of MIN/MAX value that we are looking for to one behind the start of mask
    int indexOfTargetValue = firstIndex - maskOneHalfLength;
    //Starting value is set in base class in regards to algType
    int targetValue = this->startingValue;
    for(int i = firstIndex; i <= lastIndex; i++){
        int leftMostIndexOfMask = i - maskOneHalfLength;
        //Protects us against going out of bounds from left side
        if(leftMostIndexOfMask < 0){
            leftMostIndexOfMask = 0;
        }
        //Protects us against going out of bounds from right side
        int rightMostIndexOfMask = i + maskOneHalfLength;
        if(rightMostIndexOfMask >= signalSize){
            rightMostIndexOfMask = signalSize - 1;
        }
        //If index is outside of our window mask, we need to iterate through every number to find the MIN/MAX
        if(indexOfTargetValue < leftMostIndexOfMask || i == firstIndex){
            targetValue = this->startingValue;
            for(int j = leftMostIndexOfMask; j <= rightMostIndexOfMask; j++){
                if(this->compare(this->data[j], targetValue)){
                    targetValue = this->data[j];
                    indexOfTargetValue = j;
                }
            }
            //If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
            //is more suitable than already found MIN/MAX value
        } else {
            if(this->compare(this->data[rightMostIndexOfMask], targetValue)){
                targetValue = this->data[rightMostIndexOfMask];
                indexOfTargetValue = rightMostIndexOfMask;
            }
        }
        newPartOfSignal->pushBack(targetValue);
    }
}