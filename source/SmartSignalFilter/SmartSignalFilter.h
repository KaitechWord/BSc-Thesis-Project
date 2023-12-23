#pragma once
#include "../SignalFilter/SignalFilter.h"

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
		void filter(const Signal& oldSignal, std::shared_ptr<Signal> newPartOfSignal, int firstIndex, int lastIndex, PartOfSignal partOfSignal, std::shared_ptr<std::atomic<bool>>  isPreviousDoneWithEndPart, std::shared_ptr<std::atomic<bool>>  amIDoneWithEndPart);
		std::map<int, std::map<int, int>> prePostFixes;
		std::mutex prePostFixesL;
};