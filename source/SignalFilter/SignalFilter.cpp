#include "SignalFilter.h"
#include <iostream>
#include <string>

SignalFilter::SignalFilter(int threadNum, AlgorithmType algType, int maskSize) 
            : Filter(threadNum, algType), maskSize(maskSize)
{}