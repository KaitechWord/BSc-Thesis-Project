#pragma once
#include "../ImageFilter/ImageFilter.h"

class SmartImageFilter : public ImageFilter {
public:
    SmartImageFilter(int threadNum, AlgorithmType algType, int maskSize);
    void apply(cv::Mat& image) override;
private:
    void filter(std::shared_ptr<cv::Mat> newPartOfSignal, int firstIndex, int lastIndex);
};
