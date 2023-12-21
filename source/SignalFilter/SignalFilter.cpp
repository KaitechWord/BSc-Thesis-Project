#include "SignalFilter.h"
#include <iostream>
#include <string>

SignalFilter::SignalFilter(int threadNum, AlgorithmType algType, int maskSize) 
            : Filter(threadNum, algType), maskSize(maskSize)
        {}

        void SignalFilter::apply(Signal &signal){
            int size = signal.getSignalSize();
            int threadsNum = this->tp.getThreadsNum(); //dziwnie to sie zachowuje dla wiekszej liczby watkow
            if(threadsNum > size){
                std::cout << "The number of threads is bigger than the size of the signal. Setting number of threads to size.\n";
                threadsNum = size;
            }
            int sizeOfOneThread = size / threadsNum;
            int remainder = size % threadsNum;
            std::vector<std::shared_ptr<Signal>> partsOfSignal;

            this->tp.start();
            for(int i = 0; i < threadsNum; i++){
                int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
                int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
                partsOfSignal.emplace_back(std::make_shared<Signal>());
                std::shared_ptr<Signal> partOfSignal = partsOfSignal.back();
                this->tp.queueJob([this, &signal, partOfSignal, firstIndex, lastIndex]() { this->filter(signal, partOfSignal, firstIndex, lastIndex); });
            }
            while (this->tp.busy()) {};
            this->tp.stop();
            for(int i = 0; i < threadsNum; i++){
                int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
                int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
                int partOfSignalIndex = 0;
                for(int j = firstIndex; j <= lastIndex; j++){
                    signal[j] = partsOfSignal[i]->getSignal()[partOfSignalIndex++];
                }
            }
        }