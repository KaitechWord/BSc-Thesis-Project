#include "FileManager.h"
#include <stdexcept>
#include <sstream>
#include <iostream>

void FileManager::openFile(const std::string &fileName, FileType fileType){
    if(fileType == FileType::INPUT_FILE){
        inputSignalFile.open(fileName, std::ifstream::in);
    } else{
        outputSignalFile.open(fileName, std::ifstream::out);
    }
    try{
        if(this->inputSignalFile.is_open() && fileType == FileType::INPUT_FILE){
            return;
        } else if(this->outputSignalFile.is_open() && fileType == FileType::OUTPUT_FILE){
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
    signal.setSignal(this->signal);
}

void FileManager::saveSignalToFile(Signal &signal, const std::string& fileName) {
    this->openFile(fileName, FileType::OUTPUT_FILE);
    for (const auto& signalElement : signal.getSignal()) {
        this->outputSignalFile << signalElement << " ";
    }
}

FileManager::~FileManager(){
    inputSignalFile.close();
    outputSignalFile.close();
}