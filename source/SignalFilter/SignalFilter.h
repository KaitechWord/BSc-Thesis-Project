#pragma once
#include "../Filter/Filter.h"
#include <vector>

class SignalFilter : public Filter<std::vector<uint8_t>>{
    public:
        SignalFilter(int threadNum, AlgorithmType algType, int maskSize);
    protected:
        int maskSize;
};