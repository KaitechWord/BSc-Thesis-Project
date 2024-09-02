#include "FileManager/FileManager.h"
#include "Config/Config.h"
#include "Config/FilterInfo.h"
#include "NaiveSignalFilter/NaiveSignalFilter.h"
#include "SmartSignalFilter/SmartSignalFilter.h"

#include <catch2/catch_test_macros.hpp>

Config& config = Config::instance(std::string(ROOT_DIR) + "/data/config.json");

void copySignalToMat(std::vector<int>& signal, cv::Mat& signalMat) {
	for (int i = 0; i < signal.size(); ++i) {
		signalMat.at<uchar>(0, i) = signal[i];
	}
}

void printDifferences(cv::Mat& lhs, cv::Mat& rhs) {
	for (int i = 0; i < lhs.size().width; ++i) {
		if (lhs.at<uchar>(0, i) != rhs.at<uchar>(0, i)) {
			std::cout << "LHS vs RHS diff at: " << i << ".\n" << static_cast<int>(lhs.at<uchar>(0, i)) << " vs. " << static_cast<int>(rhs.at<uchar>(0, i)) << "\n\n";
		}
	}
}

TEST_CASE("NaiveSignalFilter") {
	FilterInfo signalInfo;
	config.getInfo(signalInfo, InfoToRead::SIGNAL);
	FileManager fileManager;
	fileManager.loadSignalFromFile(signalInfo.dataPath);
	std::vector<int> signal;
	fileManager.getLoadedSignal(signal);
	cv::Mat signalTestMat(1, signal.size(), CV_8UC1);
	copySignalToMat(signal, signalTestMat);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(signalInfo.maskSize, 1));

	SECTION("MIN") {
		cv::erode(signalTestMat, signalTestMat, kernel);
		SECTION("Single thread") {
			NaiveSignalFilter naiveSignalFilter(1, AlgorithmType::MIN, signalInfo.maskSize);
			naiveSignalFilter.apply(signal);
			cv::Mat signalMat(1, signal.size(), CV_8UC1);
			copySignalToMat(signal, signalMat);
			const auto signalsAreTheSameSize = signalMat.size() == signalTestMat.size();
			cv::Mat check;
			cv::bitwise_xor(signalMat, signalTestMat, check);
			const auto signalsAreEqual = cv::countNonZero(check) == 0;
			if (!signalsAreEqual)
				printDifferences(signalMat, signalTestMat);
			REQUIRE((signalsAreTheSameSize && signalsAreEqual));
		}

		SECTION("Multiple threads") {
			NaiveSignalFilter naiveSignalFilter(signalInfo.threadsNum, AlgorithmType::MIN, signalInfo.maskSize);
			naiveSignalFilter.apply(signal);
			cv::Mat signalMat(1, signal.size(), CV_8UC1);
			copySignalToMat(signal, signalMat);
			const auto signalsAreTheSameSize = signalMat.size() == signalTestMat.size();
			cv::Mat check;
			cv::bitwise_xor(signalMat, signalTestMat, check);
			const auto signalsAreEqual = cv::countNonZero(check) == 0;
			if (!signalsAreEqual)
				printDifferences(signalMat, signalTestMat);
			REQUIRE((signalsAreTheSameSize && signalsAreEqual));
		}
	}
	SECTION("MAX") {
		cv::dilate(signalTestMat, signalTestMat, kernel);
		SECTION("Single thread") {
			NaiveSignalFilter naiveImageFilter(1, AlgorithmType::MAX, signalInfo.maskSize);
			naiveImageFilter.apply(signal);
			cv::Mat signalMat(1, signal.size(), CV_8UC1);
			copySignalToMat(signal, signalMat);
			const auto signalsAreTheSameSize = signalMat.size() == signalTestMat.size();
			cv::Mat check;
			cv::bitwise_xor(signalMat, signalTestMat, check);
			const auto signalsAreEqual = cv::countNonZero(check) == 0;
			if (!signalsAreEqual)
				printDifferences(signalMat, signalTestMat);
			REQUIRE((signalsAreTheSameSize && signalsAreEqual));
		}
		SECTION("Multiple threads") {
			NaiveSignalFilter naiveImageFilter(signalInfo.threadsNum, AlgorithmType::MAX, signalInfo.maskSize);
			naiveImageFilter.apply(signal);
			cv::Mat signalMat(1, signal.size(), CV_8UC1);
			copySignalToMat(signal, signalMat);
			const auto signalsAreTheSameSize = signalMat.size() == signalTestMat.size();
			cv::Mat check;
			cv::bitwise_xor(signalMat, signalTestMat, check);
			const auto signalsAreEqual = cv::countNonZero(check) == 0;
			if (!signalsAreEqual)
				printDifferences(signalMat, signalTestMat);
			REQUIRE((signalsAreTheSameSize && signalsAreEqual));
		}
	}
}

TEST_CASE("SmartSignalFilter") {
	FilterInfo signalInfo;
	config.getInfo(signalInfo, InfoToRead::SIGNAL);
	FileManager fileManager;
	fileManager.loadSignalFromFile(signalInfo.dataPath);
	std::vector<int> signal;
	fileManager.getLoadedSignal(signal);
	cv::Mat signalTestMat(1, signal.size(), CV_8UC1);
	copySignalToMat(signal, signalTestMat);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(signalInfo.maskSize, 1));

	SECTION("MIN") {
		cv::erode(signalTestMat, signalTestMat, kernel);
		SECTION("Single thread") {
			SmartSignalFilter smartSignalFilter(1, AlgorithmType::MIN, signalInfo.maskSize);
			smartSignalFilter.apply(signal);
			cv::Mat signalMat(1, signal.size(), CV_8UC1);
			copySignalToMat(signal, signalMat);
			const auto signalsAreTheSameSize = signalMat.size() == signalTestMat.size();
			cv::Mat check;
			cv::bitwise_xor(signalMat, signalTestMat, check);
			const auto signalsAreEqual = cv::countNonZero(check) == 0;
			if (!signalsAreEqual)
				printDifferences(signalMat, signalTestMat);
			REQUIRE((signalsAreTheSameSize && signalsAreEqual));
		}

		SECTION("Multiple threads") {
			SmartSignalFilter smartSignalFilter(signalInfo.threadsNum, AlgorithmType::MIN, signalInfo.maskSize);
			smartSignalFilter.apply(signal);
			cv::Mat signalMat(1, signal.size(), CV_8UC1);
			copySignalToMat(signal, signalMat);
			const auto signalsAreTheSameSize = signalMat.size() == signalTestMat.size();
			cv::Mat check;
			cv::bitwise_xor(signalMat, signalTestMat, check);
			const auto signalsAreEqual = cv::countNonZero(check) == 0;
			if (!signalsAreEqual)
				printDifferences(signalMat, signalTestMat);
			REQUIRE((signalsAreTheSameSize && signalsAreEqual));
		}
	}
	SECTION("MAX") {
		cv::dilate(signalTestMat, signalTestMat, kernel);
		SECTION("Single thread") {
			SmartSignalFilter smartSignalFilter(1, AlgorithmType::MAX, signalInfo.maskSize);
			smartSignalFilter.apply(signal);
			cv::Mat signalMat(1, signal.size(), CV_8UC1);
			copySignalToMat(signal, signalMat);
			const auto signalsAreTheSameSize = signalMat.size() == signalTestMat.size();
			cv::Mat check;
			cv::bitwise_xor(signalMat, signalTestMat, check);
			const auto signalsAreEqual = cv::countNonZero(check) == 0;
			if (!signalsAreEqual)
				printDifferences(signalMat, signalTestMat);
			REQUIRE((signalsAreTheSameSize && signalsAreEqual));
		}
		SECTION("Multiple threads") {
			SmartSignalFilter smartSignalFilter(signalInfo.threadsNum, AlgorithmType::MAX, signalInfo.maskSize);
			smartSignalFilter.apply(signal);
			cv::Mat signalMat(1, signal.size(), CV_8UC1);
			copySignalToMat(signal, signalMat);
			const auto signalsAreTheSameSize = signalMat.size() == signalTestMat.size();
			cv::Mat check;
			cv::bitwise_xor(signalMat, signalTestMat, check);
			const auto signalsAreEqual = cv::countNonZero(check) == 0;
			if (!signalsAreEqual)
				printDifferences(signalMat, signalTestMat);
			REQUIRE((signalsAreTheSameSize && signalsAreEqual));
		}
	}
}