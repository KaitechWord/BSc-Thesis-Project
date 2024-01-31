#include "Config.h"
#include <iostream>
#include <string>

Config::Config(const std::string &filePath)
{
    boost::property_tree::read_json(filePath, pt);
}

Config& Config::instance(const std::string &filePath){
    static Config instance(filePath);
    return instance;
}

void Config::getInfo(FilterInfo &filterInfo, InfoToRead type){
    std::string variant, approach;
    filterInfo.threadsNum = this->pt.get<int>("ThreadsNumber");
    switch (type){
        case InfoToRead::SIGNAL:
            variant = this->pt.get<std::string>("SignalFilter.Variant");
            approach = this->pt.get<std::string>("SignalFilter.Approach");
            filterInfo.dataPath = this->pt.get<std::string>("SignalFilter.DataPath");
            filterInfo.maskSize = this->pt.get<int>("SignalFilter.MaskSize");
            break;
        case InfoToRead::IMAGE:
            variant = this->pt.get<std::string>("ImageFilter.Variant");
            approach = this->pt.get<std::string>("ImageFilter.Approach");
            filterInfo.dataPath = this->pt.get<std::string>("ImageFilter.DataPath");
            filterInfo.maskSize = this->pt.get<int>("ImageFilter.MaskSideSize");
            break;
    }
    this->verifyInfo(filterInfo, variant, approach);
}

void Config::verifyInfo(FilterInfo &filterInfo, std::string &variant, std::string &approach){
    std::transform(variant.begin(), variant.end(), variant.begin(), [](unsigned char c){ return std::toupper(c); });
    if(variant == "MAX"){
        filterInfo.variant = AlgorithmType::MAX;
    } else if(variant == "MIN"){
        filterInfo.variant = AlgorithmType::MIN;
    } else{
        std::cout << "Unexpected algorithm variant.\nAccepted possibilities are: min / max\nSetting it to MAX\n";
        filterInfo.variant = AlgorithmType::MAX; 
    }
    std::transform(approach.begin(), approach.end(), approach.begin(), [](unsigned char c){ return std::toupper(c); });
    if(approach == "NAIVE"){
        filterInfo.approach = FilterApproach::NAIVE;
    } else if(approach == "SMART"){
        filterInfo.approach = FilterApproach::SMART;
    } else{
        std::cout << "Unexpected approach type.\nAccepted possibilities are: naive / smart\nSetting it to SMART.\n";
        filterInfo.approach = FilterApproach::SMART; 
    }
    if(filterInfo.maskSize <= 0){
        std::cout << "Mask (side) size less or equal to 0.\nMask (side) size set to 3.\n";
        filterInfo.maskSize = 3;
    }
    if(filterInfo.maskSize % 2 == 0){
        filterInfo.maskSize--;
        std::cout << "Mask (side) size is even.\nMask (side) needs to be odd to work correctly (to establish centre pixel). Setting it to one lower: " << filterInfo.maskSize << "\n";

    }
}

Config::~Config(){}