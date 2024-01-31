#include "Filter.h"
#include "../Signal/Signal.h"
#include <opencv2/core.hpp>

template<typename T>
Filter<T>::Filter(int threadNum, AlgorithmType algType)
    : algType(algType), tp(threadNum)
{
    switch(this->algType){
        case AlgorithmType::MIN:
            this->compare = [](int a, int b) -> bool { return a < b; };
            this->startingValue = INT_MAX;
            break;
        case AlgorithmType::MAX:
            this->compare = [](int a, int b) -> bool { return a > b; };
            this->startingValue = INT_MIN;
            break;
    }
}

template class Filter<Signal>;
template class Filter<cv::Mat>;