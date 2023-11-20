#include "Config.h"
#include <iostream>

Config::Config(const std::string &filePath)
{
    boost::property_tree::read_json(filePath, pt);
}

Config& Config::instance(const std::string &filePath){
    static Config instance(filePath);
    return instance;
}

void Config::getInfo(FilterInfo &filterInfo, InfoToRead type){
    switch (type){
        case InfoToRead::SIGNAL:
            //these need to be double checked
            filterInfo.variant = this->pt.get<std::string>("SignalFilter.Variant");
            filterInfo.approach = this->pt.get<std::string>("SignalFilter.Approach");
            filterInfo.dataPath = this->pt.get<std::string>("SignalFilter.DataPath");
            break;
        case InfoToRead::IMAGE:
            filterInfo.variant = pt.get<std::string>("ImageFilter.Variant");
            filterInfo.approach = pt.get<std::string>("ImageFilter.Approach");
            filterInfo.dataPath = pt.get<std::string>("ImageFilter.DataPath");
            break;
        
    }
}

void Config::verifyInfo(FilterInfo &filterInfo){
    std::transform(filterInfo.variant.begin(), filterInfo.variant.end(), filterInfo.variant.begin(), [](unsigned char c){ return std::tolower(c); });
    if(filterInfo.variant != "min" || filterInfo.variant != "max"){
        std::cout << "Unexpected algorithm variant.\nAccepted possibilities are: min / max\nSetting it to min\n";
        filterInfo.variant = "min";
    }
    std::transform(filterInfo.approach.begin(), filterInfo.approach.end(), filterInfo.approach.begin(), [](unsigned char c){ return std::tolower(c); });
    if(filterInfo.approach != "naive" || filterInfo.variant != "smart"){
        std::cout << "Unexpected approach type.\nAccepted possibilities are: naive / smart\nSetting it to naive\n";
        filterInfo.approach = "naive";
    }
}

Config::~Config(){}