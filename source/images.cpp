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
    cv::Mat dst = ogImage;
    NaiveImageFilter NIF(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize);
    NIF.apply(image);
    fileManager.saveImageToFile(image, "./imageResult.png");

    // TESTING - MIN VARIANT
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(imageInfo.maskSize, imageInfo.maskSize));
    cv::erode(ogImage, ogImage, kernel);
    cv::bitwise_xor(ogImage, image, dst);
    if (cv::countNonZero(dst) > 0) {
        std::cout << "not same";
    }
    else {
        std::cout << "same";
    }
    // ~TESTING
    return 0;
}
