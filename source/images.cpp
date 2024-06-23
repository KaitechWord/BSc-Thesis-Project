#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <filesystem>
#include "FileManager/FileManager.h"
#include "Config/Config.h"
#include "Config/FilterInfo.h"
#include "NaiveImageFilter/NaiveImageFilter.h"
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>

Config& config = Config::instance("./data/config.json");

int main(int argc, char* argv[]) {
    FilterInfo imageInfo;
    config.getInfo(imageInfo, InfoToRead::IMAGE);
    FileManager fileManager;
    fileManager.loadImageFromFile(imageInfo.dataPath);
    cv::Mat image;
    fileManager.getLoadedImage(image);

    // TESTING PURPOSES
    fileManager.loadImageFromFile(imageInfo.dataPath);
    cv::Mat testImage;
    fileManager.getLoadedImage(testImage);
    // ~TESTING PURPOSES

    NaiveImageFilter NIF(imageInfo.threadsNum, imageInfo.variant, imageInfo.maskSize);
    NIF.apply(image);
    fileManager.saveImageToFile(image, "./imageResult.png");

    // TESTING - MIN VARIANT ONLY
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(imageInfo.maskSize, imageInfo.maskSize));
    cv::erode(testImage, testImage, kernel);
    cv::imshow("changed", image);
    cv::imshow("test", testImage);
    cv::waitKey(10000);

    //Uncomment one line below to fail the test for sure
    //testImage.at<uchar>(0, 0) = '1';
    bool areDifferent = false;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (image.at<uchar>(i, j) != testImage.at<uchar>(i, j)) {
                std::cout << static_cast<int>(image.at<uchar>(i, j)) << " vs. " << static_cast<int>(testImage.at<uchar>(i, j)) << " at (" << i << ", " << j << ")\n";
                areDifferent = true;
                break;
            }
        }
        if (areDifferent) {
            break;
        }
    }
    if (areDifferent) {
        std::cout << "\nTest failed - images are different\n\n";
    } else {
        std::cout << "\nTest succeeded - images are equal\n\n";
    }
    // ~TESTING

    return 0;
}
