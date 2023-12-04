#include "FileManager.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

void FileManager::openFile(const std::string &fileName, FileType fileType){
    if(fileType == FileType::INPUT_FILE){
        inputSignalFile.open(fileName);
    } else{
        outputSignalFile.open(fileName);
    }
    try{
        if(inputSignalFile.is_open() && fileType == FileType::INPUT_FILE){
            return;
        } else if(inputSignalFile.is_open() && fileType == FileType::OUTPUT_FILE){
            return;
        }
        throw std::runtime_error("Couldn't open file. Provided file path: " + fileName);
    } catch (const std::runtime_error& e){
        throw;
    } 
}

void FileManager::loadSignalFromFile(const std::string &fileName){
    this->openFile(fileName, FileType::INPUT_FILE);
    std::string line;
    while(std::getline(this->inputSignalFile, line)){
        std::stringstream lineS(line);
        int number;
        while(lineS >> number){
            signal.push_back(number);
        }
    }
}

void FileManager::getLoadedSignal(Signal &signal){
    int signalSize = this->signal.size();
    signal.setSize(signalSize);
    for(int i = 0; i < signalSize; i++){
        signal[i] = this->signal[i];
    }
}

void FileManager::saveSignalToFile(const Signal &signal, const std::string& fileName) {
    this->openFile(fileName, FileType::OUTPUT_FILE);
    // Write each element of the array to the file
    for (int i = 0; i < signal.getSize(); i++) {
        this->outputSignalFile << signal[i] << " ";
    }
}

FileManager::~FileManager(){
    inputSignalFile.close();
    outputSignalFile.close();
}