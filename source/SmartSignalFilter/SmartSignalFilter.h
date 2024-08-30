#pragma once
#include "../SignalFilter/SignalFilter.h"

class SmartSignalFilter : public SignalFilter {
	public:
		SmartSignalFilter(int threadNum, AlgorithmType algType, int maskSize);
		void apply(std::vector<int>& signal) override;
	private:
		void filter(std::vector<int>& newPartOfSignal, int firstIndex, int lastIndex);
};