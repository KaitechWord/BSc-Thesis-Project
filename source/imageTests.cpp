#include "FileManager/FileManager.h"
#include "Config/Config.h"
#include "Config/FilterInfo.h"
#include "NaiveImageFilter/NaiveImageFilter.h"
#include "SmartImageFilter/SmartImageFilter.h"

#include <catch2/catch_test_macros.hpp>

Config& config = Config::instance(std::string(ROOT_DIR) + "/data/config.json");

void printDifferences(const cv::Mat& image1, const cv::Mat& image2){
	for (int y = 0; y < image1.rows; y++) {
        for (int x = 0; x < image1.cols; x++) {
            int pixel1 = image1.at<uchar>(y, x);
            int pixel2 = image2.at<uchar>(y, x);

            // Check if the pixels are different
            if (pixel1 != pixel2 && y != 2846 && y != 2845 /*&& y != 6 && y != 5 && y != 4&& y != 3&& y != 2*/) {
                std::cout << "Difference at (" << x << ", " << y << "): ";
                std::cout << "Image1: " << pixel1 << " Image2: " << pixel2 << std::endl;
            }
        }
    }
}

TEST_CASE("NaiveImageFilter") {
	FilterInfo imageInfo;
	config.getInfo(imageInfo, InfoToRead::IMAGE);
	FileManager fileManager;
	fileManager.loadImageFromFile(imageInfo.dataPath);
	cv::Mat image;
	image = fileManager.getLoadedImage();

	cv::Mat testImage;
	testImage = cv::imread(imageInfo.dataPath);
	cv::cvtColor(testImage, testImage, cv::COLOR_BGR2GRAY);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(imageInfo.maskSize, imageInfo.maskSize));

	SECTION("MIN") {
		cv::erode(testImage, testImage, kernel);
		SECTION("Single thread") {
			NaiveImageFilter naiveImageFilter(1, AlgorithmType::MIN, imageInfo.maskSize);
			naiveImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = (cv::sum(image != testImage) == cv::Scalar(0,0,0,0));
			REQUIRE(imagesAreTheSameSize);
			// printDifferences(testImage, image);
			REQUIRE(imagesAreEqual);
		}

		SECTION("Multiple threads") {
			NaiveImageFilter naiveImageFilter(imageInfo.threadsNum, AlgorithmType::MIN, imageInfo.maskSize);
			naiveImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = (cv::sum(image != testImage) == cv::Scalar(0,0,0,0));
			REQUIRE(imagesAreTheSameSize);
			// printDifferences(testImage, image);
			REQUIRE(imagesAreEqual);
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
			const auto imagesAreEqual = (cv::sum(image != testImage) == cv::Scalar(0,0,0,0));
			REQUIRE(imagesAreTheSameSize);
			// printDifferences(testImage, image);
			REQUIRE(imagesAreEqual);
		}
		SECTION("Multiple threads") {
			NaiveImageFilter naiveImageFilter(imageInfo.threadsNum, AlgorithmType::MAX, imageInfo.maskSize);
			naiveImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = (cv::sum(image != testImage) == cv::Scalar(0,0,0,0));
			REQUIRE(imagesAreTheSameSize);
			if(!imagesAreEqual)
			// printDifferences(testImage, image);
			REQUIRE(imagesAreEqual);
		}
	}
}

TEST_CASE("SmartImageFilter") {
	FilterInfo imageInfo;
	config.getInfo(imageInfo, InfoToRead::IMAGE);
	FileManager fileManager;
	fileManager.loadImageFromFile(imageInfo.dataPath);
	cv::Mat image;
	image = fileManager.getLoadedImage();

	cv::Mat testImage;
	testImage = cv::imread(imageInfo.dataPath);
	cv::cvtColor(testImage, testImage, cv::COLOR_BGR2GRAY);
	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(imageInfo.maskSize, imageInfo.maskSize));

	SECTION("MIN") {
		cv::erode(testImage, testImage, kernel);
		SECTION("Single thread") {
			SmartImageFilter smartImageFilter(1, AlgorithmType::MIN, imageInfo.maskSize);
			smartImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = (cv::sum(image != testImage) == cv::Scalar(0,0,0,0));
			REQUIRE(imagesAreTheSameSize);
			//printDifferences(testImage, image);
			REQUIRE(imagesAreEqual);
		}

		SECTION("Multiple threads") {
			SmartImageFilter smartImageFilter(imageInfo.threadsNum, AlgorithmType::MIN, imageInfo.maskSize);
			smartImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = (cv::sum(image != testImage) == cv::Scalar(0,0,0,0));
			REQUIRE(imagesAreTheSameSize);
			//printDifferences(testImage, image);
			REQUIRE(imagesAreEqual);
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
			const auto imagesAreEqual = (cv::sum(image != testImage) == cv::Scalar(0,0,0,0));
			REQUIRE(imagesAreTheSameSize);
			//printDifferences(testImage, image);
			REQUIRE(imagesAreEqual);
		}
		SECTION("Multiple threads") {
			SmartImageFilter smartImageFilter(imageInfo.threadsNum, AlgorithmType::MAX, imageInfo.maskSize);
			smartImageFilter.apply(image);
			const auto imagesAreTheSameSize = image.size() == testImage.size();
			cv::Mat check;
			cv::bitwise_xor(image, testImage, check);
			const auto imagesAreEqual = (cv::sum(image != testImage) == cv::Scalar(0,0,0,0));
			REQUIRE(imagesAreTheSameSize);
			printDifferences(testImage, image);
			REQUIRE(imagesAreEqual);
		}
	}
}