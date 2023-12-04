#pragma once
#include "../SignalFilter/SignalFilter.h"

class NaiveSignalFilter : public SignalFilter{
    public:
        NaiveSignalFilter(int threadNum, AlgorithmType algType, int maskSize);
    private:
        void filter(const Signal& oldSignal, Signal& newPartOfSignal, int firstIndex, int lastIndex) override;
};
