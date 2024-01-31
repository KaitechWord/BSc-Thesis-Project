#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include "FileManager/FileManager.h"
#include "./Config/Config.h"
#include "./Config/FilterInfo.h"
#include "./NaiveImageFilter/NaiveImageFilter.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

Config& config = Config::instance("../../../data/config.json");

int main(int argc, char* argv[]) {
    FilterInfo imageInfo;
    config.getInfo(imageInfo, InfoToRead::IMAGE);
    FileManager fileManager;
    fileManager.loadImageFromFile(imageInfo.dataPath);
    cv::Mat image;
    fileManager.getLoadedImage(image);
    cv::Mat ogImage = image;
    NaiveImageFilter NIF(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize);
    NIF.apply(image);
    fileManager.saveImageToFile(image, "./imageResult.png");
    return 0;
}
