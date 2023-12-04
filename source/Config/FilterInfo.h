#pragma once
#include <string>
#include "../Filter/Filter.h"

struct FilterInfo{
    int threadsNum;
    AlgorithmType variant;
    FilterApproach approach;
    std::string dataPath;
    int maskSize;
};