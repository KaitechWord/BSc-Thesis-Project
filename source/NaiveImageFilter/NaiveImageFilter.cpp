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
    int threadsNum = 1;//this->tp.getThreadsNum(); //dziwnie to sie zachowuje dla wiekszej liczby watkow
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
        partsOfImage.emplace_back(std::make_shared<cv::Mat>());
        std::shared_ptr<cv::Mat> partOfImage = partsOfImage.back();
        this->tp.queueJob([this, partOfImage, firstIndex, lastIndex]() { this->filter(partOfImage, firstIndex, lastIndex); });
    }
    while (this->tp.busy()) {};
    this->tp.stop();
    for (int i = 0; i < threadsNum; i++) {
        int firstIndex = i * sizeOfOneThread + std::min(i, remainder);
        int lastIndex = (i + 1) * sizeOfOneThread + std::min(i + 1, remainder) - 1;
        int partOfimageIndex = 0;
        for (int j = firstIndex; j <= lastIndex; j++) {
            image[j] = partsOfImage[i][partOfimageIndex++];
        }
    }
}

void NaiveImageFilter::filter(std::shared_ptr<cv::Mat> newPartOfimage, int firstIndex, int lastIndex) {
    int imageSize = this->data.rows * this->data.cols;
    int rowSize = this->data.rows;
    int colSize = this->data.cols;
    int firstIndexRow = firstIndex % rowSize;
    int firstIndexCol = firstIndex / rowSize;
    int lastIndexRow = lastIndex % rowSize;
    int lastIndexCol = lastIndex / rowSize;
    //By one half I mean the half without the center point, e.g. maskSize = 5, thus the half length is 2
    int maskOneHalfLength = this->maskSize / 2;
    //Starting value is set in base class in regards to algType
    int targetValue = this->startingValue;
    auto t1 = std::chrono::high_resolution_clock::now();
    //Setting the initial value of index of MIN/MAX value that we are looking for to one behind the start of mask
    int rowOfTargetValue = firstIndexRow - maskOneHalfLength;
    int colOfTargetValue = firstIndexCol - maskOneHalfLength;
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

            int upMostIndexOfMask = i - maskOneHalfLength;
            if (upMostIndexOfMask < 0) {
                rightMostIndexOfMask = 0;
            }

            int downMostIndexOfMask = i + maskOneHalfLength;
            //Protects us against going out of bounds from right side
            if (downMostIndexOfMask >= rowSize) {
                downMostIndexOfMask = rowSize - 1;
            }
            
            if (colOfTargetValue < leftMostIndexOfMask || 
                colOfTargetValue > rightMostIndexOfMask ||
                rowOfTargetValue < upMostIndexOfMask ||
                rowOfTargetValue > downMostIndexOfMask ||
                (i == firstIndexRow && j == firstIndexCol)) {
                targetValue = this->startingValue;
                for (int k = upMostIndexOfMask; k <= downMostIndexOfMask; k++) {
                    for (int l = leftMostIndexOfMask; l <= rightMostIndexOfMask; l++) {

                        if (this->compare(this->data.at<int>(k, l), targetValue)) {
                            targetValue = this->data.at<int>(k, l);
                            rowOfTargetValue = k;
                            colOfTargetValue = l;
                        }
                    }
                }
                //If index is inside the mask, we need to check if the new value that entered the mask by moving it by one value
                //is more suitable than already found MIN/MAX value
            } else {
                //tutaj mozna by wprowadzic dodatkowa zmienna ktora przetrzymuje ostatni row / col i sprawdzac czy ktores z tego sie zmienilo
                //jesli ostatni col sie zmienil to przeszukujemy tylko nowa kolumne ktora weszla po przesunieciu sie o jeden w prawo,
                //a jesli ostatni row sie zmienil wzgledem aktualnego to przeszukujemy tylko nowy rzad, ktory wszedl po przesunieciu o jeden w dol
                for (int k = ) {

                }
                if (this->compare(this->data.at<int>[down][rightMostIndexOfMask], targetValue)) {
                    targetValue = this->data[rightMostIndexOfMask];
                    indexOfTargetValue = rightMostIndexOfMask;
                }
            }
            newPartOfSignal->pushBack(targetValue);
        }
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms_double = t2 - t1;
    std::cout << ms_double.count() << "ms\n";
}