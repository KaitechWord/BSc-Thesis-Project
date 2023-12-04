#include "Filter.h"
#include "../Signal/Signal.h"

template<typename T>
Filter<T>::Filter(int threadNum, AlgorithmType algType)
    : algType(algType), tp(threadNum)
{
    switch(this->algType){
        case AlgorithmType::MIN:
            this->compare = [](int a, int b) -> bool { return a < b; };
            this->startingValue = INT_MAX;
        case AlgorithmType::MAX:
            this->compare = [](int a, int b) -> bool { return a > b; };
            this->startingValue = INT_MIN;
    }
}

template class Filter<Signal>;