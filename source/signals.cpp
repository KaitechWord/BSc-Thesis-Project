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
    FilterInfo signalInfo;
    if (argc == 1) {
		    config.getInfo(signalInfo, InfoToRead::SIGNAL);
	}
	else if (argc == 6) {
		signalInfo.threadsNum = std::stoi(argv[1]);
		signalInfo.variant = std::stoi(argv[2]) == static_cast<int>(AlgorithmType::MIN) ? AlgorithmType::MIN : AlgorithmType::MAX;
		signalInfo.approach = std::stoi(argv[3]) == static_cast<int>(FilterApproach::NAIVE) ? FilterApproach::NAIVE : FilterApproach::SMART;
		signalInfo.dataPath = argv[4];
		signalInfo.maskSize = std::stoi(argv[5]);
	}
	else {
		std::cerr << "Invalid number of arguments! The argumentas are: <threads_num> <variant> <approach> <file_path> <mask_size>";
		return 1;
	}
    FileManager fileManager;
    fileManager.loadSignalFromFile(signalInfo.dataPath);
    std::vector<uint8_t> signal;
    fileManager.getLoadedSignal(signal);
    std::unique_ptr<SignalFilter> signalFilter = signalInfo.approach == FilterApproach::NAIVE ? std::unique_ptr< SignalFilter >(std::make_unique<NaiveSignalFilter>(signalInfo.threadsNum, signalInfo.variant, signalInfo.maskSize))
		: std::unique_ptr< SignalFilter >(std::make_unique<SmartSignalFilter>(signalInfo.threadsNum, signalInfo.variant, signalInfo.maskSize));
	signalFilter->apply(signal);
    fileManager.saveSignalToFile(signal, std::string(ROOT_DIR) + "/output/signalResult.txt");
    return 0;
}
