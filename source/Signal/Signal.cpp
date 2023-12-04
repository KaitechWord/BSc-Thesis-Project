#include "Signal.h"
#include <stdexcept>
#include <string>

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
    this->size = signal.size;
    this->data = new int[this->size];
    for(int i = 0; i < this->size; i++){
        this->data[i] = signal.data[i];
    }
    return *this;
}

int Signal::operator[](int i) const{
    if(i < 0 || i >= this->size){
        throw new std::runtime_error("Index " + std::to_string(i) + " is out of bounds. Max. size: " + std::to_string(this->size));
    }
    return this->data[i];
}

int& Signal::operator[](int i){
    if(i < 0 || i >= this->size){
        throw new std::runtime_error("Index " + std::to_string(i) + " is out of bounds. Max. size: " + std::to_string(this->size));
    }
    return this->data[i];
}

void Signal::setSignal(int *data){
    this->data = data;
    data = nullptr;
}

void Signal::setSize(int size){
    delete[] this->data;
    this->data = new int[size]; 
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