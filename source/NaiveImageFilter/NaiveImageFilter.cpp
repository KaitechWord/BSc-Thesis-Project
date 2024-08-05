#include "NaiveImageFilter.h"
#include <iostream>
#include <chrono>

NaiveImageFilter::NaiveImageFilter(int threadNum, AlgorithmType algType, int maskSize)
    : ImageFilter(threadNum, algType, maskSize)
{}

void NaiveImageFilter::apply(cv::Mat& image) {
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
        int colSize = this->data.cols;
        for (int j = firstIndex; j <= lastIndex; j++) {
            image.at<uchar>(j / colSize, j % colSize) = partsOfImage[i]->at<uchar>(j / colSize, j % colSize);
        }
    }
}

void NaiveImageFilter::filter(std::shared_ptr<cv::Mat> newPartOfimage, int firstIndex, int lastIndex) {
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
    int rowOfTargetValue = firstIndexRow - maskOneHalfLength;
    int colOfTargetValue = firstIndexCol - maskOneHalfLength;
    int lastRow = firstIndexRow;
    int lastCol = firstIndexCol;
    int lastLeftMostIndexOfMask = -1;
    int lastRightMostIndexOfMask = -1;
    int lasttopMostIndexOfMask = -1;
    int lastbotMostIndexOfMask = -1;
    for (int i = firstIndexRow; i < rowSize; i++) {
        for (int j = firstIndexCol; j < colSize; j++) {
            int leftMostIndexOfMask = j - maskOneHalfLength;
            //Protects us against going out of bounds from left side
            if (leftMostIndexOfMask < 0) {
                leftMostIndexOfMask = 0;
            }
            
            int rightMostIndexOfMask = j + maskOneHalfLength;
            //Protects us against going out of bounds from right side
            if (rightMostIndexOfMask >= colSize) {
                rightMostIndexOfMask = colSize - 1;
            }

            int topMostIndexOfMask = i - maskOneHalfLength;
            if (topMostIndexOfMask < 0) {
                topMostIndexOfMask = 0;
            }

            int botMostIndexOfMask = i + maskOneHalfLength;
            //Protects us against going out of bounds from right side
            if (botMostIndexOfMask >= rowSize) {
                botMostIndexOfMask = rowSize - 1;
            }
            
            if (colOfTargetValue < leftMostIndexOfMask || 
                colOfTargetValue > rightMostIndexOfMask ||
                rowOfTargetValue < topMostIndexOfMask ||
                rowOfTargetValue > botMostIndexOfMask ||
                (i == firstIndexRow && j == firstIndexCol)) {
                targetValue = this->startingValue;
                for (int k = topMostIndexOfMask; k <= botMostIndexOfMask; k++) {
                    for (int l = leftMostIndexOfMask; l <= rightMostIndexOfMask; l++) {
                        if (this->compare(static_cast<int>(this->data.at<uchar>(k, l)), targetValue)) {
                            targetValue = static_cast<int>(this->data.at<uchar>(k, l));
                            rowOfTargetValue = k;
                            colOfTargetValue = l;
                        }
                    }
                }
                //If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
                //is more suitable than already found MIN/MAX value
            } else {
                //if row changed, we need to check every pixel that is not overlapping with previous mask to find the new minimum and we check if the row really changed
                //or we are shortening the mask once again due to the bottom border
                if (lastbotMostIndexOfMask < botMostIndexOfMask) {
                    for (int k = topMostIndexOfMask; k < botMostIndexOfMask; k++) {
                        for (int l = leftMostIndexOfMask; l < lastLeftMostIndexOfMask; l++) {
                            if (this->compare(static_cast<int>(this->data.at<uchar>(k, l)), targetValue)) {
                                targetValue = static_cast<int>(this->data.at<uchar>(k, l));
                                rowOfTargetValue = k;
                                colOfTargetValue = l;
                            }
                        }
                    }
                    for (int l = leftMostIndexOfMask; l < rightMostIndexOfMask; l++) {
                        if (this->compare(static_cast<int>(this->data.at<uchar>(botMostIndexOfMask, l)), targetValue)) {
                            targetValue = static_cast<int>(this->data.at<uchar>(botMostIndexOfMask, l));
                            rowOfTargetValue = botMostIndexOfMask;
                            colOfTargetValue = l;
                        }
                    }
                }



                ////if row didn't change, we know that col must have changed, but we need to check if there is really a new col or we are just shortening the mask
                ////due to the fact that right image border stops us and if current right most index is greater than the last one, we need to check the new column that entered the mask
               if (lastRightMostIndexOfMask < rightMostIndexOfMask) {
                    for (int k = topMostIndexOfMask; k <= botMostIndexOfMask; k++) {
                        if (this->compare(static_cast<int>(this->data.at<uchar>(k, rightMostIndexOfMask)), targetValue)) {
                            targetValue = static_cast<int>(this->data.at<uchar>(k, rightMostIndexOfMask));
                            rowOfTargetValue = k;
                            colOfTargetValue = rightMostIndexOfMask;
                        }
                    }
                }
            }
            lastLeftMostIndexOfMask = leftMostIndexOfMask;
            lastRightMostIndexOfMask = rightMostIndexOfMask;
            lasttopMostIndexOfMask = topMostIndexOfMask;
            lastbotMostIndexOfMask = botMostIndexOfMask;

            newPartOfimage->at<uchar>(i, j) = static_cast<uchar>(targetValue);
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms\n";
}