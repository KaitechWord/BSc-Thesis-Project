#include "NaiveSignalFilter.h"
#include <iostream>

NaiveSignalFilter::NaiveSignalFilter(int threadNum, AlgorithmType algType, int maskSize)
    : SignalFilter(threadNum, algType, maskSize)
{}

void NaiveSignalFilter::filter(const Signal& oldSignal, Signal& newPartOfSignal, int firstIndex, int lastIndex){
    int signalSize = oldSignal.getSize();
    // std::cout << "new size " << newPartOfSignal.getSize() << std::endl;
    //By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
    int maskOneHalfLength = this->maskSize / 2;
    //Setting the initial value of index of MIN/MAX value that we are looking for to one behind the start of mask
    int indexOfTargetValue = firstIndex - maskOneHalfLength;
    //Starting value is set in base class in regards to algType
    int targetValue = this->startingValue;
    int newPartOfSignalIndex = 0;
    for(int i = firstIndex; i <= lastIndex; i++){
        // std::cout << "current index i: " << i << std::endl;
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
        // std::cout << "leftMostIndexOfMask: " << leftMostIndexOfMask << std::endl;
        // std::cout << "rightMostIndexOfMask: " << rightMostIndexOfMask << std::endl;
        //If index is outside of our window mask, we need to iterate through every number to find the MIN/MAX
        // std::cout << "indexoftargetvalue: " << indexOfTargetValue << std::endl;
        if(indexOfTargetValue < leftMostIndexOfMask){
            // std::cout << "index outside" << std::endl;
            targetValue = this->startingValue;
            for(int j = leftMostIndexOfMask; j <= rightMostIndexOfMask; j++){
                if(this->compare(oldSignal[j], targetValue)){
                    targetValue = oldSignal[j];
                    indexOfTargetValue = j;
                    // std::cout << "Max: " << targetValue << std::endl;
                    // std::cout << "MaxInd: " << indexOfTargetValue << std::endl;
                }
            }
            // std::cout << "Max: " << oldSignal[indexOfTargetValue] << std::endl;
            // std::cout << "MaxInd: " << indexOfTargetValue << std::endl;
          //If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
          //is more suitable than already found MIN/MAX value
        } else {
            // std::cout << "index inside" << std::endl;
            if(this->compare(oldSignal[rightMostIndexOfMask], targetValue)){
                targetValue = oldSignal[rightMostIndexOfMask];
                indexOfTargetValue = rightMostIndexOfMask;
                // std::cout << "Max: " << oldSignal[rightMostIndexOfMask] << std::endl;
                // std::cout << "MaxInd: " << rightMostIndexOfMask << std::endl;
            }
        }
        // std::cout << "current lowest for that: " << targetValue << std::endl << std::endl;
        // std::cout << std::endl << std::endl;
        newPartOfSignal[newPartOfSignalIndex++] = targetValue;
    }
    // std::cout << "\n";
}