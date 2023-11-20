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
        std::string word;
        while(lineS >> word){
            signal.push_back(std::stoi(word));
        }
    }
}

void FileManager::getLoadedSignal(std::vector<int> &signals){
    signals = this->signal; 
}

FileManager::~FileManager(){
    inputSignalFile.close();
    outputSignalFile.close();
}