#include "SmartSignalFilter.h"
#include <iostream>

SmartSignalFilter::SmartSignalFilter(int threadNum, AlgorithmType algType, int maskSize)
    : SignalFilter(threadNum, algType, maskSize)
{}

void SmartSignalFilter::apply(Signal& signal) {
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
    std::map<int, std::map<int, int>> prePostFixes;
    std::mutex prePostFixesL;
    for (int i = 0; i < threadsNum; i++) {
        PartOfSignal partOfSignalType = PartOfSignal::MID;
        int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
        int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
        partsOfSignal.emplace_back(std::make_shared<Signal>());
        std::shared_ptr<Signal> partOfSignal = partsOfSignal.back();
        if(threadsNum == 1){
            partOfSignalType = PartOfSignal::WHOLE;
        } else if (firstIndex == 0) {
            partOfSignalType = PartOfSignal::START;
        } else if (lastIndex == size - 1) {
            partOfSignalType = PartOfSignal::END;
        } else { //MID
        }
        this->tp.queueJob([this, partOfSignal, firstIndex, lastIndex, partOfSignalType]() { this->filter(partOfSignal, firstIndex, lastIndex, partOfSignalType); });
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
void SmartSignalFilter::filter(std::shared_ptr<Signal> newPartOfSignal, int firstIndex, int lastIndex, PartOfSignal partOfSignalType) {
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
        bool wasPrefixMaskStoppedEarlier = false;
        bool wasPostfixMaskStoppedEarlier = false;
        int stoppedEarlierLastIndex = -1;
        //prefixes
        //TODO: w zasadzie to musze zamienic tylko nazwy prefix z postfix, ale idea jest okej
        //it also tells us about the last index that was used inside the loop
        for (int startOfPrefixMask = 0; startOfPrefixMask < signalSize; startOfPrefixMask += maskSize + 1) {
            //case when the start of the prefix mask is the only left index 
            if (startOfPrefixMask == signalSize - 1) {
                // we break coz startOfPrefixMask is the only index left (we calculate the next index, which does not exist at this point)
                break;
            }
            int endOfPrefixMask = startOfPrefixMask + this->maskSize - 1;
            //case when the end of the prefix mask is bigger than size
            if (endOfPrefixMask >= signalSize) {
                //endOfPrefixMask = signalSize - 1;
                // when the prefix(postfix in real) mask ends after the size of our signal,
                // it would be the best to implement approach from naive solution, coz we cant even calc
                // one mask at this point
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

            //postfixes
            //TODO: sprawdzic te przypadki kiedy sygnal nam sie konczy wczesniej niz te indeksy od masek
            int startOfPostfixMask = startOfPrefixMask + maskSize;
            if (startOfPostfixMask >= signalSize) {
                startOfPostfixMask = signalSize - 1;
                // it means that we can only calc one index, which is the middle index of previous mask
                // thus we need to handle the indexes after the middle index of previous mask,
                // probably the naive approach would be the best once again
            }
            int endOfPostfixMask = startOfPrefixMask + maskSize * 2 - 1;
            if (endOfPostfixMask >= signalSize) {
                endOfPostfixMask = signalSize - 1;
                // it means that we can calculate the indexes(by that i mean the middle indexes of masks) up to
                // middle of prefix(postfix really) mask and up to some index which is defined by the last index of 
                // endOfPostfixMask (not directly, it helps define it in some way)
                // thus, the approach to calc the indexes after the middle one of prefix(postfix really) mask needs to be implemented
                // it can be done in a way that we use smart approach as long as we can and then switch to naive
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
            std::cout << "End of iteration" << std::endl;
        }

        // now we can proceed to deal with the mid part of a signal
        //TODO: wyjebac
        for (int i = 1; i * this->maskSize * 2 < signalSize; i++) {
            //prefixes
            int startOfPrefixMask = i * this->maskSize * 2;
            if (startOfPrefixMask >= signalSize) {
                startOfPrefixMask = signalSize - 1;
            }
            int endOfPrefixMask = i * this->maskSize * 2 + this->maskSize - 1;
            if (endOfPrefixMask >= signalSize) {
                endOfPrefixMask = signalSize - 1;
            }
            this->prePostFixes[endOfPrefixMask][endOfPrefixMask] = this->data[endOfPrefixMask];
            int lastBeforeLastPrefixMaskIndex = i * this->maskSize * 2 + this->maskSize - 2;
            if (lastBeforeLastPrefixMaskIndex >= signalSize) {
                lastBeforeLastPrefixMaskIndex = signalSize - 2; //coz we check j + 1
            }
            for (int j = lastBeforeLastPrefixMaskIndex; j >= startOfPrefixMask; j--) {
                if (this->compare(this->data[j], this->prePostFixes[j + 1][endOfPrefixMask])) {
                    this->prePostFixes[j][endOfPrefixMask] = this->data[j];
                } else {
                    this->prePostFixes[j][endOfPrefixMask] = this->prePostFixes[j + 1][endOfPrefixMask];
                }
            }
            //postfixes
            int startOfPostfixMask = i * this->maskSize * 2 + maskSize;
            if (startOfPostfixMask >= signalSize) {
                startOfPostfixMask = signalSize - 1;
            }
            int endOfPostfixMask = i * this->maskSize * 2 + maskSize * 2 - 1;
            if (endOfPostfixMask >= signalSize) {
                endOfPostfixMask = signalSize - 1;
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
                int leftMostIndexOfMask = j - maskOneHalfLength;
                //Protects us against going out of bounds from left side
                if (leftMostIndexOfMask < 0) {
                    leftMostIndexOfMask = 0;
                }
                //I think that might not be needed since we restrict it in j loop values
                int rightMostIndexOfMask = j + maskOneHalfLength;
                if (rightMostIndexOfMask > endOfPostfixMask) {
                    rightMostIndexOfMask = endOfPostfixMask;
                }
                
                //if right index is in postfix part, we need to properly use prefix and postfix part
                if (j == startOfPostfixMask) {
                    if (this->compare(this->prePostFixes[leftMostIndexOfMask][j-1], this->prePostFixes[j][rightMostIndexOfMask])) {
                        newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][j - 1]);
                    } else {
                        newPartOfSignal->pushBack(this->prePostFixes[j][rightMostIndexOfMask]);
                    }
                } else if (rightMostIndexOfMask >= startOfPostfixMask) {
                    if (this->compare(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask], this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask])) {
                        newPartOfSignal->pushBack(this->prePostFixes[leftMostIndexOfMask][endOfPrefixMask]);
                    } else {
                        newPartOfSignal->pushBack(this->prePostFixes[startOfPostfixMask][rightMostIndexOfMask]);
                    }
                } else {
                    newPartOfSignal->pushBack(this->prePostFixes[rightMostIndexOfMask][leftMostIndexOfMask]);
                }
                std::cout << newPartOfSignal->getSignal().back() << std::endl;
            } 
        }
    }
}