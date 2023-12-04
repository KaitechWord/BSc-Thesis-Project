#include "SignalFilter.h"
#include <iostream>
#include <string>

SignalFilter::SignalFilter(int threadNum, AlgorithmType algType, int maskSize) 
            : Filter(threadNum, algType), maskSize(maskSize)
        {}

        void SignalFilter::apply(Signal &signal){
            this->data = signal;
            int size = signal.getSize();
            int threadsNum = this->tp.getThreadsNum();
            if(threadsNum > size){
                std::cout << "The number of threads is bigger than the size of the signal. Setting number of threads to size.\n";
                threadsNum = size;
            }
            int sizeOfOneThread = size / threadsNum;
            int remainder = size % threadsNum;
            Signal *partsOfSignal = new Signal[threadsNum];
            this->tp.start();
            for(int i = 0; i < threadsNum; i++){
                int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
                int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
                // std::cout << "First index: " << firstIndex << std::endl;
                // std::cout << "Last index: "  << lastIndex << std::endl;
                partsOfSignal[i] = Signal(lastIndex - firstIndex + 1);
                this->tp.queueJob([this, signal, partsOfSignal, i, firstIndex, lastIndex](){ this->filter(signal, partsOfSignal[i], firstIndex, lastIndex); } );
            }
            this->tp.stop();
            for(int i = 0; i < threadsNum; i++){
                int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
                int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
                int partOfSignalIndex = 0;
                for(int j = firstIndex; j <= lastIndex; j++){
                    // std::cout << partsOfSignal[i][partOfSignalIndex] << " ";
                    signal[j] = partsOfSignal[i][partOfSignalIndex++];
                }
            }
        }