#pragma once
#include "../SignalFilter/SignalFilter.h"

class SmartSignalFilter : public SignalFilter {
	public:
		SmartSignalFilter(int threadNum, AlgorithmType algType, int maskSize);
		void apply(std::vector<uint8_t>& signal) override;
	private:
		void filter(std::vector<uint8_t>& newPartOfSignal, int firstIndex, int lastIndex);
};