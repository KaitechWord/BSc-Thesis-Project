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

Config &config = Config::instance("../../../data/config.json");

//TODO:
// There gonna be two apps(two projects in cmake)
// One for Signal algorithms
// One for Image algorithms
// I have got Filter and inheritated ImageFilter and SignalFilter
// use template so the virtual function like applyFilter or smth can be override
// with proper variable types in children classes like in 
// https://www.modernescpp.com/index.php/surprise-included-inheritance-and-member-functions-of-class-templates/
// https://stackoverflow.com/questions/8810224/inheriting-from-a-template-class-in-c
int main(int argc, char *argv[]){
    FilterInfo signalInfo, imageInfo;
    config.getInfo(signalInfo, InfoToRead::SIGNAL);
    FileManager fileManager;
    fileManager.loadSignalFromFile(signalInfo.dataPath);
    Signal signal;
    fileManager.getLoadedSignal(signal);
    SmartSignalFilter SSF(signalInfo.threadsNum, signalInfo.variant, signalInfo.maskSize);
    SSF.apply(signal);
    fileManager.saveSignalToFile(signal, "./yikes.txt");
    return 0;
}
