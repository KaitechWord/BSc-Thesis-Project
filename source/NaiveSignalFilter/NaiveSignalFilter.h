#pragma once
#include "../SignalFilter/SignalFilter.h"

class NaiveSignalFilter : public SignalFilter{
    public:
        NaiveSignalFilter(int threadNum, AlgorithmType algType, int maskSize);
        void apply(Signal& signal) override;
    private:
        void filter(std::shared_ptr<Signal> newPartOfSignal, int firstIndex, int lastIndex);
};
