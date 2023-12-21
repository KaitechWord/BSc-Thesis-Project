#include "Signal.h"
#include <stdexcept>
#include <string>

void Signal::pushBack(int value) {
    this->data.push_back(value);
}

int Signal::getSignalSize() const{
    return this->data.size();
}

void Signal::setSignal(const std::vector<int>& signal) {
    this->data = signal;
}

const std::vector<int> Signal::getSignal() const{
    return this->data;
}

int Signal::operator[](int i) const{
    if(i < 0 || i >= this->data.size()){
        throw new std::runtime_error("Index " + std::to_string(i) + " is out of bounds. Max. size: " + std::to_string(this->data.size()));
    }
    return this->data[i];
}

int& Signal::operator[](int i){
    if(i < 0 || i >= this->data.size()){
        throw new std::runtime_error("Index " + std::to_string(i) + " is out of bounds. Max. size: " + std::to_string(this->data.size()));
    }
    return this->data[i];
}

Signal::~Signal(){}