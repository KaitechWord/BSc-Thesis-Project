#pragma once

class Signal{
    public:
        Signal();
        Signal(int size);
        Signal& operator=(const Signal &signal);
        int operator[](int i) const;
        int& operator[](int i);
        void setSignal(int *data);
        void setSize(int size);
        int* getSignal();
        int getSize() const;
        ~Signal();
    private:
        int size;
        int* data;
};