#include "Filter.h"
#include "../Signal/Signal.h"
#include <opencv2/core.hpp>

template<typename T>
Filter<T>::Filter(int threadsNum, AlgorithmType algType)
	: algType(algType), threadNum(threadsNum)
{
	switch (this->algType) {
	case AlgorithmType::MIN:
		this->compare = [](uint8_t a, uint8_t b) -> bool { return a < b; };
		this->startingValue = UINT8_MAX;
		break;
	case AlgorithmType::MAX:
		this->compare = [](uint8_t a, uint8_t b) -> bool { return a > b; };
		this->startingValue = 0;
		break;
	}
}

template class Filter<std::vector<uint8_t>>;
template class Filter<cv::Mat>;