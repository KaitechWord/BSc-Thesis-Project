#pragma once
#include <climits>
#include "../ThreadPool/ThreadPool.h"

enum class AlgorithmType{
    MIN,
    MAX
};

enum class FilterApproach{
    NAIVE,
    SMART
};

template<typename T>
class Filter{
    public:
        Filter(int threadNum, AlgorithmType algType);
        virtual void apply(T &data) = 0;
    protected:
        T data;
        AlgorithmType algType;
        ThreadPool tp;
        //So the action is applied to first int, e.g. if the compare
        //is MIN, if the first int is less than second, it returns true etc.
        std::function<bool(int, int)> compare;
        int startingValue;
};