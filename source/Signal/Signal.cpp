#include "Signal.h"
#include <stdexcept>

Signal::Signal()
    : size(0), data(nullptr)
{}

Signal::Signal(int size)
    : size(size), data(new int[size])
{
    if(data == nullptr){
        throw std::runtime_error("Wrong Signal memory allocation");
    }
}

Signal& Signal::operator=(const Signal &signal){
    if(this != &signal){
        delete[] this->data; 
    } else {
        return *this;
    }
    this->data = new int[signal.size];
    for(int i = 0; i < signal.size; i++){
        this->data[i] = signal.data[i];
    }
}

void Signal::setSignal(int *data){
    this->data = data;
    data = nullptr;
}

void Signal::setSize(int size){
    this->size = size;
}

int* Signal::getSignal(){
    return this->data;
}

int Signal::getSize() const{
    return this->size;
}

Signal::~Signal(){
    delete[] this->data;
}