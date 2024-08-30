#pragma once
#include "../Filter/Filter.h"
#include "../Signal/Signal.h"
#include <vector>

class SignalFilter : public Filter<std::vector<int>>{
    public:
        SignalFilter(int threadNum, AlgorithmType algType, int maskSize);
    protected:
        int maskSize;
};