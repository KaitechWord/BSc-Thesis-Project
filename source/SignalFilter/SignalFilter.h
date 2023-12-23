#pragma once
#include "../Filter/Filter.h"
#include "../Signal/Signal.h"
#include <vector>

class SignalFilter : public Filter<Signal>{
    public:
        SignalFilter(int threadNum, AlgorithmType algType, int maskSize);
    protected:
        int maskSize;
};