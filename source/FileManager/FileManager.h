#pragma once
#include <fstream>
#include <vector>
#include "../Signal/Signal.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

enum class FileType {
    INPUT_FILE,
    OUTPUT_FILE
};

class FileManager{
    public:
        void loadSignalFromFile(const std::string &fileName);
        void loadImageFromFile(const std::string& fileName);
        void saveSignalToFile(Signal& signal, const std::string& fileName);
        void saveImageToFile(cv::Mat &image, const std::string& fileName);
        void getLoadedSignal(Signal &signal);
        void getLoadedImage(cv::Mat& image);
        ~FileManager();
    private:
        void openFile(const std::string &fileName, FileType fileType);
        std::ifstream inputSignalFile;
        std::ofstream outputSignalFile;
        std::vector<int> signal;
        cv::Mat image;
};