#pragma once
#include <vector>
#include <mutex>

class Signal{
    public:

        void pushBack(int value);
        int operator[](int i) const;
        int& operator[](int i);
        int getSignalSize() const;
        std::vector<int> getSignal();
        void setSignal(const std::vector<int> &signal);
        ~Signal();
    private:
        std::vector<int> data;
};