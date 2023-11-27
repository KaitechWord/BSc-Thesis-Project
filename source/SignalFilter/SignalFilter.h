#pragma once
#include "../Filter/Filter.h"
#include "../Signal/Signal.h"
#include <vector>


class SignalFilter : public Filter<Signal>{
    public:
        SignalFilter(int threadNum, int maskSize) 
            : Filter(threadNum), maskSize(maskSize)
        {}

        Signal apply(const Signal &signal) override{
            this->data = signal;
            int threadsNum = this->tp.getThreadsNum();
            Signal *partsOfSignal = new Signal[this->tp.getThreadsNum()];
            for(int i = 0; i < threadsNum; i++){
                partsOfSignal[i] = Signal(signal.getSize());                                     //the logic for first and last index needs to be
                                                                                                 //first that comes to mind is: size / threadsNum = floor(k)
                                                                                                 //then first index (of given part) is: i * floor(k)
                                                                                                 //then last index (of given part) is: (i + 1) * floor(k) - 1
                                                                                                 //e.g. there is an error when size is 9 and threadsNum is 2, then floor(k) = 4
                                                                                                 // i = 0 -> firsIndex = i * k = 0 * 4 = 0
                                                                                                 //          lastIndex = (i + 1) * k - 1 = 1 * 4 - 1 = 3 
                                                                                                 // i = 1 -> firsIndex = i * k = 1 * 4 = 4
                                                                                                 //          lastIndex = (i + 1) * k - 1 = 2 * 4 - 1 = 7
                                                                                                 // non covered index is 8, the logic to cover that needs to be implemented

                tp.queueJob([this, partsOfSignal, i](){ this->filter(this->data, partsOfSignal[i], 0, 0); } );
            }
            return signal;
        }
        
    private:
        int maskSize;
        void filter(const Signal& oldSignal, Signal& newPartOfSignal, int firstIndex, int lastIndex){
            //tobeimplemented
        }
};