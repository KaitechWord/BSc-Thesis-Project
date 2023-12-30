#pragma once
#include <fstream>
#include <vector>
#include "../Signal/Signal.h"

enum class FileType {
    INPUT_FILE,
    OUTPUT_FILE
};

class FileManager{
    public:
        void loadSignalFromFile(const std::string &fileName);
        void saveSignalToFile(Signal &signal, const std::string& fileName);
        void getLoadedSignal(Signal &signal);
        ~FileManager();
    private:
        void openFile(const std::string &fileName, FileType fileType);
        std::ifstream inputSignalFile;
        std::ofstream outputSignalFile;
        std::vector<int> signal;
};