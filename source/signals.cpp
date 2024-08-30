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

Config &config = Config::instance(std::string(ROOT_DIR) + "/data/config.json");

int main(int argc, char *argv[]){
    FilterInfo signalInfo, imageInfo;
    config.getInfo(signalInfo, InfoToRead::SIGNAL);
    FileManager fileManager;
    fileManager.loadSignalFromFile(signalInfo.dataPath);
    std::vector<int> signal;
    fileManager.getLoadedSignal(signal);
    SmartSignalFilter SSF(signalInfo.threadsNum, signalInfo.variant, signalInfo.maskSize);
    //NaiveSignalFilter SSF(signalInfo.threadsNum, signalInfo.variant, signalInfo.maskSize);
    SSF.apply(signal);
    fileManager.saveSignalToFile(signal, std::string(ROOT_DIR) + "/output/signalResult.txt");
    return 0;
}
