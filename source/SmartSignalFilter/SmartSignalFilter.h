#pragma once
#include "../SignalFilter/SignalFilter.h"
#include <map>

enum class PartOfSignal {
	START,
	MID,
	END,
	WHOLE
};

class SmartSignalFilter : public SignalFilter {
	public:
		SmartSignalFilter(int threadNum, AlgorithmType algType, int maskSize);
		void apply(Signal& signal) override;
	private:
		void filter(std::shared_ptr<Signal> newPartOfSignal, int firstIndex, int lastIndex, PartOfSignal partOfSignalType);
		std::unordered_map<int, std::unordered_map<int, int>> prePostFixes;
		std::mutex prePostFixesL;
};