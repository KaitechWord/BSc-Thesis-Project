#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include "FileManager/FileManager.h"
#include "./Config/Config.h"
#include "./Config/FilterInfo.h"
#include "./Signal/Signal.h"
#include "./NaiveSignalFilter/NaiveSignalFilter.h"
#include "./SmartSignalFilter/SmartSignalFilter.h"

Config &config = Config::instance("../data/config.json");

int main(int argc, char *argv[]){
    FilterInfo signalInfo, imageInfo;
    config.getInfo(signalInfo, InfoToRead::SIGNAL);
    FileManager fileManager;
    fileManager.loadSignalFromFile(signalInfo.dataPath);
    Signal signal;
    fileManager.getLoadedSignal(signal);
    //SmartSignalFilter SSF(signalInfo.threadsNum, signalInfo.variant, signalInfo.maskSize);
    NaiveSignalFilter SSF(signalInfo.threadsNum, signalInfo.variant, signalInfo.maskSize);
    SSF.apply(signal);
    fileManager.saveSignalToFile(signal, "./yikes.txt");
    return 0;
}
