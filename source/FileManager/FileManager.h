#pragma once
#include <fstream>
#include <vector>

enum class FileType {
    INPUT_FILE,
    OUTPUT_FILE
};

class FileManager{
    public:
        void loadSignalFromFile(const std::string &fileName);
        void getLoadedSignal(std::vector<int> &signal);
        ~FileManager();
    private:
        void openFile(const std::string &fileName, FileType fileType);
        std::ifstream inputSignalFile;
        std::ifstream outputSignalFile;
        std::vector<int> signal;
};