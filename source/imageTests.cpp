#include "FileManager/FileManager.h"
#include "Config/Config.h"
#include "Config/FilterInfo.h"
#include "NaiveImageFilter/NaiveImageFilter.h"
#include "SmartImageFilter/SmartImageFilter.h"

#include <catch2/catch_test_macros.hpp>

Config& config = Config::instance(std::string(ROOT_DIR) + "/data/config.json");

TEST_CASE("NaiveImageFilter") {
	FilterInfo imageInfo;
	config.getInfo(imageInfo, InfoToRead::IMAGE);
	FileManager fileManager;
	fileManager.loadImageFromFile(imageInfo.dataPath);
	cv::Mat image;
	fileManager.getLoadedImage(image);

	cv::Mat testImage;
	fileManager.getLoadedImage(testImage);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(imageInfo.maskSize, imageInfo.maskSize));

	SECTION("MIN") {
		cv::erode(testImage, testImage, kernel);
		SECTION("Single thread") {
			NaiveImageFilter naiveImageFilter(1, AlgorithmType::MIN, imageInfo.maskSize);
			naiveImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = cv::countNonZero(check) == 0;
			REQUIRE((imagesAreTheSameSize && imagesAreEqual));
		}

		SECTION("Multiple threads") {
			NaiveImageFilter naiveImageFilter(imageInfo.threadsNum, AlgorithmType::MIN, imageInfo.maskSize);
			naiveImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = cv::countNonZero(check) == 0;
			REQUIRE((imagesAreTheSameSize && imagesAreEqual));
		}
	}
	SECTION("MAX") {
		cv::dilate(testImage, testImage, kernel);
		SECTION("Single thread") {
			NaiveImageFilter naiveImageFilter(1, AlgorithmType::MAX, imageInfo.maskSize);
			naiveImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = cv::countNonZero(check) == 0;
			REQUIRE((imagesAreTheSameSize && imagesAreEqual));
		}
		SECTION("Multiple threads") {
			NaiveImageFilter naiveImageFilter(imageInfo.threadsNum, AlgorithmType::MAX, imageInfo.maskSize);
			naiveImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = cv::countNonZero(check) == 0;
			REQUIRE((imagesAreTheSameSize && imagesAreEqual));
		}
	}
}

TEST_CASE("SmartImageFilter") {
	FilterInfo imageInfo;
	config.getInfo(imageInfo, InfoToRead::IMAGE);
	FileManager fileManager;
	fileManager.loadImageFromFile(imageInfo.dataPath);
	cv::Mat image;
	fileManager.getLoadedImage(image);

	cv::Mat testImage;
	fileManager.getLoadedImage(testImage);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(imageInfo.maskSize, imageInfo.maskSize));

	SECTION("MIN") {
		cv::erode(testImage, testImage, kernel);
		SECTION("Single thread") {
			SmartImageFilter smartImageFilter(1, AlgorithmType::MIN, imageInfo.maskSize);
			smartImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = cv::countNonZero(check) == 0;
			REQUIRE((imagesAreTheSameSize && imagesAreEqual));
		}

		SECTION("Multiple threads") {
			SmartImageFilter smartImageFilter(imageInfo.threadsNum, AlgorithmType::MIN, imageInfo.maskSize);
			smartImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = cv::countNonZero(check) == 0;
			REQUIRE((imagesAreTheSameSize && imagesAreEqual));
		}
	}
	SECTION("MAX") {
		cv::dilate(testImage, testImage, kernel);
		SECTION("Single thread") {
			SmartImageFilter smartImageFilter(1, AlgorithmType::MAX, imageInfo.maskSize);
			smartImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = cv::countNonZero(check) == 0;
			REQUIRE((imagesAreTheSameSize && imagesAreEqual));
		}
		SECTION("Multiple threads") {
			SmartImageFilter smartImageFilter(imageInfo.threadsNum, AlgorithmType::MAX, imageInfo.maskSize);
			smartImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = cv::countNonZero(check) == 0;
			REQUIRE((imagesAreTheSameSize && imagesAreEqual));
		}
	}
}