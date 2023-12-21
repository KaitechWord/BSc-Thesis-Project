#pragma once
#include <vector>

class Signal{
    public:
        void pushBack(int value);
        int operator[](int i) const;
        int& operator[](int i);
        int getSignalSize() const;
        const std::vector<int> getSignal() const;
        void setSignal(const std::vector<int> &signal);
        ~Signal();
    private:
        std::vector<int> data;
};