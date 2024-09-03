#pragma once
#include "../SignalFilter/SignalFilter.h"

class NaiveSignalFilter : public SignalFilter{
    public:
        NaiveSignalFilter(int threadNum, AlgorithmType algType, int maskSize);
        void apply(std::vector<uint8_t>& signal) override;
    private:
        void filter(std::vector<uint8_t>& newSignal, int firstIndex, int lastIndex);
};
