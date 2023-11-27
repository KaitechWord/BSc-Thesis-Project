#pragma once

class Signal{
    public:
        Signal();
        Signal(int size);
        Signal& operator=(const Signal &signal);
        void setSignal(int *data);
        void setSize(int size);
        int* getSignal();
        int getSize() const;
        ~Signal();
    private:
        int size;
        int* data;
};