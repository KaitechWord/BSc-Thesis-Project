#pragma once
#include "../Filter/Filter.h"
#include "../Signal/Signal.h"
#include <vector>

class SignalFilter : public Filter<Signal>{
    public:
        SignalFilter(int threadNum, AlgorithmType algType, int maskSize);
        void apply(Signal &signal) override;
    protected:
        int maskSize;
        virtual void filter(const Signal& oldSignal, std::shared_ptr<Signal> newPartOfSignal, int firstIndex, int lastIndex) = 0;
};