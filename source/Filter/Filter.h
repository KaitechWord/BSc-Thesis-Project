#pragma once
#include <climits>
#include <functional>

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
        Filter(int threadsNum, AlgorithmType algType);
        virtual void apply(T &data) = 0;
    protected:
        T data;
        AlgorithmType algType;
        int threadNum;
        //So the action is applied to first int, e.g. if the compare
        //is MIN, if the first int is less than second, it returns true etc.
        std::function<bool(uint8_t, uint8_t)> compare;
        uint8_t startingValue;
};