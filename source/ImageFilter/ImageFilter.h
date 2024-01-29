#pragma once
#include "../Filter/Filter.h"
#include <opencv2/opencv.hpp>
#include <vector>

class ImageFilter : public Filter<cv::Mat> {
public:
    ImageFilter(int threadNum, AlgorithmType algType, int maskSize);
protected:
    int maskSize;
};