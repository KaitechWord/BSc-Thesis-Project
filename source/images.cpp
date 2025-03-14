#include <filesystem>
#include "FileManager/FileManager.h"
#include "Config/Config.h"
#include "Config/FilterInfo.h"
#include "NaiveImageFilter/NaiveImageFilter.h"
#include "MixImageFilter/MixImageFilter.h"
#include "SmartImageFilter/SmartImageFilter.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

Config &config = Config::instance(std::string(ROOT_DIR) + "/data/config.json");

int main(int argc, char *argv[])
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LogLevel::LOG_LEVEL_SILENT);
	FilterInfo imageInfo;
	if (argc == 1)
	{
		config.getInfo(imageInfo, InfoToRead::IMAGE);
	}
	else if (argc == 6)
	{
		imageInfo.threadsNum = std::stoi(argv[1]);
		imageInfo.variant = std::stoi(argv[2]) == static_cast<int>(AlgorithmType::MIN) ? AlgorithmType::MIN : AlgorithmType::MAX;
		if (auto algType = std::stoi(argv[3]); algType == static_cast<int>(FilterApproach::NAIVE))
		{
			imageInfo.approach = FilterApproach::NAIVE;
		}
		else if (algType == static_cast<int>(FilterApproach::MIX))
		{
			imageInfo.approach = FilterApproach::MIX;
		}
		else if (algType == static_cast<int>(FilterApproach::SMART))
		{
			imageInfo.approach = FilterApproach::SMART;
		}
		imageInfo.dataPath = argv[4];
		imageInfo.maskSize = std::stoi(argv[5]);
	}
	else
	{
		std::cerr << "Invalid number of arguments! The argumentas are: <threads_num> <variant> <approach> <file_path> <mask_size>";
		return 1;
	}
	FileManager fileManager;
	fileManager.loadImageFromFile(imageInfo.dataPath);
	cv::Mat image;
	image = fileManager.getLoadedImage();

	std::unique_ptr<ImageFilter> imageFilter{nullptr};
	if (imageInfo.approach == FilterApproach::NAIVE)
	{
		imageFilter = std::unique_ptr<ImageFilter>(std::make_unique<NaiveImageFilter>(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize));
	}
	else if (imageInfo.approach == FilterApproach::MIX)
	{
		imageFilter = std::unique_ptr<ImageFilter>(std::make_unique<MixImageFilter>(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize));
	}
	else if (imageInfo.approach == FilterApproach::SMART)
	{
		imageFilter = std::unique_ptr<ImageFilter>(std::make_unique<SmartImageFilter>(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize));
	}
	imageFilter->apply(image);
	if (argc == 1)
		fileManager.saveImageToFile(image, std::string(ROOT_DIR) + "/output/imageResult.png");
	exit(0);
	return 0;
}
