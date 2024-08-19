#include "Config.h"
#include <iostream>
#include <string>
#include <fstream>

Config::Config(const std::string& filePath)
{
	json = nlohmann::json::parse(std::ifstream{filePath});
}

Config& Config::instance(const std::string& filePath) {
	static Config instance(filePath);
	return instance;
}

void Config::getInfo(FilterInfo& filterInfo, InfoToRead type) {
	std::string variant, approach;
	filterInfo.threadsNum = this->json["ThreadsNumber"];
	switch (type) {
	case InfoToRead::SIGNAL:
		variant = this->json["SignalFilter"]["Variant"];
		approach = this->json["SignalFilter"]["Approach"];
		filterInfo.dataPath = std::string(ROOT_DIR) + json["SignalFilter"]["DataPath"].get<std::string>();
		filterInfo.maskSize = json["SignalFilter"]["MaskSize"];
		break;
	case InfoToRead::IMAGE:
		variant = json["ImageFilter"]["Variant"];
		approach = json["ImageFilter"]["Approach"];
		filterInfo.dataPath = std::string(ROOT_DIR) + json["ImageFilter"]["DataPath"].get<std::string>();
		filterInfo.maskSize = json["ImageFilter"]["MaskSize"];
		break;
	}
	this->verifyInfo(filterInfo, variant, approach);
}

void Config::verifyInfo(FilterInfo& filterInfo, std::string& variant, std::string& approach) {
	std::transform(variant.begin(), variant.end(), variant.begin(), [](unsigned char c) { return std::toupper(c); });
	if (variant == "MAX") {
		filterInfo.variant = AlgorithmType::MAX;
	}
	else if (variant == "MIN") {
		filterInfo.variant = AlgorithmType::MIN;
	}
	else {
		std::cout << "Unexpected algorithm variant.\nAccepted possibilities are: min / max\nSetting it to MAX\n";
		filterInfo.variant = AlgorithmType::MAX;
	}
	std::transform(approach.begin(), approach.end(), approach.begin(), [](unsigned char c) { return std::toupper(c); });
	if (approach == "NAIVE") {
		filterInfo.approach = FilterApproach::NAIVE;
	}
	else if (approach == "SMART") {
		filterInfo.approach = FilterApproach::SMART;
	}
	else {
		std::cout << "Unexpected approach type.\nAccepted possibilities are: naive / smart\nSetting it to SMART.\n";
		filterInfo.approach = FilterApproach::SMART;
	}
	if (filterInfo.maskSize <= 0) {
		std::cout << "Mask (side) size less or equal to 0.\nMask (side) size set to 3.\n";
		filterInfo.maskSize = 3;
	}
	if (filterInfo.maskSize % 2 == 0) {
		filterInfo.maskSize--;
		std::cout << "Mask (side) size is even.\nMask (side) needs to be odd to work correctly (to establish centre pixel). Setting it to one lower: " << filterInfo.maskSize << "\n";

	}
}

Config::~Config() {}