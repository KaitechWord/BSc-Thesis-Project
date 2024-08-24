#include <filesystem>
#include "FileManager/FileManager.h"
#include "Config/Config.h"
#include "Config/FilterInfo.h"
#include "NaiveImageFilter/NaiveImageFilter.h"
#include "SmartImageFilter/SmartImageFilter.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

Config& config = Config::instance(std::string(ROOT_DIR) + "/data/config.json");

int main(int argc, char* argv[]) {
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
	FilterInfo imageInfo;
	if (argc == 1) {
		config.getInfo(imageInfo, InfoToRead::IMAGE);
	}
	else if(argc == 6){
		imageInfo.threadsNum = std::stoi(argv[1]);
		imageInfo.variant = std::stoi(argv[2]) == static_cast<int>(AlgorithmType::MIN) ? AlgorithmType::MIN : AlgorithmType::MAX;
		imageInfo.approach = std::stoi(argv[3]) == static_cast<int>(FilterApproach::NAIVE) ? FilterApproach::NAIVE : FilterApproach::SMART;
		imageInfo.dataPath = argv[4];
		imageInfo.maskSize = std::stoi(argv[5]);
	}
	else {
		std::cerr << "Invalid number of arguments! The argumentas are: <threads_num> <variant> <approach> <file_path> <mask_size>";
		return 1;
	}
	FileManager fileManager;
	fileManager.loadImageFromFile(imageInfo.dataPath);
	cv::Mat image;
	fileManager.getLoadedImage(image);

	std::unique_ptr<ImageFilter> imageFilter = imageInfo.approach == FilterApproach::NAIVE ? std::unique_ptr< ImageFilter >(std::make_unique<NaiveImageFilter>(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize))
		: std::unique_ptr< ImageFilter >(std::make_unique<SmartImageFilter>(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize));
	imageFilter->apply(image);
	fileManager.saveImageToFile(image, std::string(ROOT_DIR) + "/output/imageResult.png");

	return 0;
}
