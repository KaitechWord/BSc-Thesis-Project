#pragma once
#include "../ThreadPool/ThreadPool.h"

template<typename T>
class Filter{
    public:
        Filter(int threadNum);
        virtual T apply(const T &data) = 0;
    protected:
        T data;
        ThreadPool tp;
};