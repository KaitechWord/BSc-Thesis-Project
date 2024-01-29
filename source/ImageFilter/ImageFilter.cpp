#include "ImageFilter.h"
#include <iostream>
#include <string>

ImageFilter::ImageFilter(int threadNum, AlgorithmType algType, int maskSize)
    : Filter(threadNum, algType), maskSize(maskSize)
{}