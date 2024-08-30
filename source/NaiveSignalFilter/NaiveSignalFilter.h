#pragma once
#include "../SignalFilter/SignalFilter.h"

class NaiveSignalFilter : public SignalFilter{
    public:
        NaiveSignalFilter(int threadNum, AlgorithmType algType, int maskSize);
        void apply(std::vector<int>& signal) override;
    private:
        void filter(std::vector<int>& newSignal, int firstIndex, int lastIndex);
};
