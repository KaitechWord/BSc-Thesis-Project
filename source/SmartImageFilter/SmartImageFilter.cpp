include "SmartImageFilter.h"
#include <iostream>
#include <chrono>
#include <map>

SmartImageFilter::SmartImageFilter(int threadNum, AlgorithmType algType, int maskSize)
    : ImageFilter(threadNum, algType, maskSize)
{}

void SmartImageFilter::apply(cv::Mat& image) {
    this->data = image;
    int rowSize = image.rows;
    int colSize = image.cols;
    int pixelsNum = rowSize * colSize;
    int threadsNum = 1;//this->tp.getThreadsNum();
    if (threadsNum > pixelsNum) {
        std::cout << "The number of threads is bigger than the size of the image. Setting number of threads to size.\n";
        threadsNum = pixelsNum;
    }
    int sizeOfOneThread = pixelsNum / threadsNum;
    int remainder = pixelsNum % threadsNum;
    std::vector<std::shared_ptr<cv::Mat>> partsOfImage;

    this->tp.start();
    for (int i = 0; i < threadsNum; i++) {
        int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
        int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
        partsOfImage.emplace_back(std::make_shared<cv::Mat>(rowSize, colSize, image.type()));
        std::shared_ptr<cv::Mat> partOfImage = partsOfImage.back();
        this->tp.queueJob([this, partOfImage, firstIndex, lastIndex]() { this->filter(partOfImage, firstIndex, lastIndex); });
    }
    while (this->tp.busy()) {};
    this->tp.stop();
    for (int i = 0; i < threadsNum; i++) {
        int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
        int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
        int partOfimageIndex = 0;
        int rowSize = this->data.rows;
        for (int j = firstIndex; j <= lastIndex; j++) {
            image.at<uchar>(j / rowSize, j % rowSize) = partsOfImage[i]->at<uchar>(j / rowSize, j % rowSize);
        }
    }
}

struct MaskIndexes
{
    int topMost;
    int botMost;
    int leftMost;
    int rightMost;
}

void SmartImageFilter::filter(std::shared_ptr<cv::Mat> newPartOfimage, int firstIndex, int lastIndex) {
    int imageSize = this->data.rows * this->data.cols;
    int rowSize = this->data.rows;
    int colSize = this->data.cols;
    int firstIndexRow = firstIndex / rowSize;
    int firstIndexCol = firstIndex % rowSize;
    int lastIndexRow = lastIndex / rowSize;
    int lastIndexCol = lastIndex % rowSize;
    //By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
    int maskOneHalfLength = this->maskSize / 2;
    //Starting value is set in base class in regards to algType
    int targetValue = this->startingValue;
    auto t1 = std::chrono::high_resolution_clock::now();
    int lastRow = firstIndexRow;
    int lastCol = firstIndexCol;
    MaskIndexes firstMask, secondMask;
    //[x][y][z]:
    //  x <- starting index of array in a row
    //  y <- end index of array in a row
    //  z <- row index
    std::map<int, std::map<int, std::map<int, uchar>>> desiredValue;
    for (int i = firstIndexRow; i < rowSize; i++) {
        for (int j = firstIndexCol; j < colSize; j++) {
            //use std::clamp in following
            firstMask.leftMost = j - maskOneHalfLength;
            //Protects us against going out of bounds from left side
            if (firstMask.leftMost < 0) {
                firstMask.leftMost = 0;
            }
            
            firstMask.rightMost = j + maskOneHalfLength;
            //Protects us against going out of bounds from right side
            if (firstMask.rightMost >= colSize) {
                firstMask.rightMost = colSize - 1;
            }

            firstMask.topMost = i - maskOneHalfLength;
            if (firstMask.topMost < 0) {
                firstMask.topMost = 0;
            }

            firstMask.botMost = i + maskOneHalfLength;
            //Protects us against going out of bounds from right side
            if (firstMask.botMost >= rowSize) {
                firstMask.botMost = rowSize - 1;
            }

            secondMask.leftMost = j + this->maskSize - maskOneHalfLength;
            //Protects us against going out of bounds from left side
            if (secondMask.leftMost < 0) {
                secondMask.leftMost = 0;
            }
            
            secondMask.rightMost = j + this->maskSize + maskOneHalfLength;
            //Protects us against going out of bounds from right side
            if (secondMask.rightMost >= colSize) {
                secondMask.rightMost = colSize - 1;
            }

            //topMost and botMost not needed since it is the same for first mask
            // secondMask.topMost = i - maskOneHalfLength;
            // if (secondMask.topMost < 0) {
            //     secondMask.topMost = 0;
            // }

            // secondMask.botMost = i + maskOneHalfLength;
            // //Protects us against going out of bounds from right side
            // if (secondMask.botMost >= rowSize) {
            //     secondMask.botMost = rowSize - 1;
            // }
            
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms\n";
}