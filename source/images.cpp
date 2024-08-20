#include <filesystem>
#include "FileManager/FileManager.h"
#include "Config/Config.h"
#include "Config/FilterInfo.h"
#include "NaiveImageFilter/NaiveImageFilter.h"
#include "SmartImageFilter/SmartImageFilter.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

Config& config = Config::instance(std::string(ROOT_DIR) + "/data/config.json");

int main(int argc, char* argv[]) {
	FilterInfo imageInfo;
	config.getInfo(imageInfo, InfoToRead::IMAGE);
	FileManager fileManager;
	fileManager.loadImageFromFile(imageInfo.dataPath);
	cv::Mat image;
	fileManager.getLoadedImage(image);

	std::unique_ptr<ImageFilter> imageFilter = imageInfo.approach == FilterApproach::NAIVE ? std::unique_ptr< ImageFilter >(std::make_unique<NaiveImageFilter>(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize))
																						   : std::unique_ptr< ImageFilter >(std::make_unique<SmartImageFilter>(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize));
	imageFilter->apply(image);
	fileManager.saveImageToFile(image, std::string(ROOT_DIR) + "/imageResult.png");

	return 0;
}
