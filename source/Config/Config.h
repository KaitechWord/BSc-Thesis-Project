#pragma once
#include "FilterInfo.h"
#include <nlohmann/json.hpp>
#include <string>


enum class InfoToRead{
    SIGNAL,
    IMAGE
};

class Config{
    public:
        Config(const Config&) = delete;
        Config(Config&&) = delete;
        Config& operator=(const Config&) = delete;
        Config& operator=(Config&&) = delete;
        static Config& instance(const std::string &filePath);
        void getInfo(FilterInfo &filterInfo, InfoToRead type);
    private:
        FilterInfo info;
        Config(const std::string &filePath);
        ~Config();
        void verifyInfo(FilterInfo &filterInfo, std::string &variant, std::string &approach);
        nlohmann::json json;
};