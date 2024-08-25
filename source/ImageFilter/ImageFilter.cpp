#include "ImageFilter.h"
#include <iostream>
#include <string>

ImageFilter::ImageFilter(int threadsNum, AlgorithmType algType, int maskSize)
    : Filter(threadsNum, algType), maskSize(maskSize)
{}