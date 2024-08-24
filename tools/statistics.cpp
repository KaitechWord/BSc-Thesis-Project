#include <Config.h>
#include <iostream>
#include <fstream>

static const auto naiveMinSingleFilePath = std::string(ROOT_DIR) + "/output/NaiveMinSingle.txt";
static const auto naiveMinMultiFilePath = std::string(ROOT_DIR) + "/output/NaiveMinMulti.txt";
static const auto naiveMaxSingleFilePath = std::string(ROOT_DIR) + "/output/NaiveMaxSingle.txt";
static const auto naiveMaxMultiFilePath = std::string(ROOT_DIR) + "/output/NaiveMaxMulti.txt";

static const auto smartMinSingleFilePath = std::string(ROOT_DIR) + "/output/SmartMinSingle.txt";
static const auto smartMinMultiFilePath = std::string(ROOT_DIR) + "/output/SmartMinMulti.txt";
static const auto smartMaxSingleFilePath = std::string(ROOT_DIR) + "/output/SmartMaxSingle.txt";
static const auto smartMaxMultiFilePath = std::string(ROOT_DIR) + "/output/SmartMaxMulti.txt";

static constexpr auto filesNum = 8;

static const auto filesPaths = std::array<std::string, filesNum>{ naiveMinSingleFilePath, naiveMinMultiFilePath, naiveMaxSingleFilePath, naiveMaxMultiFilePath, smartMinSingleFilePath, smartMinMultiFilePath, smartMaxSingleFilePath, smartMaxMultiFilePath };

static const auto statisticsOutputFilePath = std::string(ROOT_DIR) + "/output/Statistics.txt";

float calculateMedian(std::vector<float>& numbers) {
	auto size = numbers.size();
	if (size == 0)
		throw std::runtime_error("No numbers to calculate median.");
	std::sort(numbers.begin(), numbers.end());
	if (size % 2 == 0)
		return (numbers.at(size / 2 - 1) + numbers.at(size / 2) / 2.0f);
	else
		return numbers.at(size / 2);
}

float calculateAverage(std::vector<float>& numbers) {
	if (numbers.empty()) {
		throw std::runtime_error("No numbers to calculate average.");
	}
	auto sum = std::accumulate(numbers.begin(), numbers.end(), 0.0f);
	return sum / numbers.size();
}

Config& config = Config::instance(std::string(ROOT_DIR) + "/data/config.json");

int main() {
	std::array<std::ifstream, filesNum> files;
	std::ofstream outputFile(statisticsOutputFilePath, std::ios_base::trunc);
	if (!outputFile.is_open()) {
		std::cerr << "Unable to open output file." << std::endl;
		return 1;
	}

	for (auto i = 0; i < filesNum; ++i) {
		std::vector<float> fileNumbers;
		auto& file = files.at(i);
		const auto& filePath = filesPaths.at(i);
		file.open(filePath, std::ios_base::beg);
		if (file.is_open())
		{
			float number;
			while (file >> number)
				fileNumbers.push_back(number);
			file.close();
			if (fileNumbers.empty()) {
				std::cerr << "File is empty or contains no valid numbers." << std::endl;
				return 1;
			}
			try {
				const auto median = calculateMedian(fileNumbers);
				const auto average = calculateAverage(fileNumbers);
				std::cout << "Median for file " << filePath << " is:\n" << median << "\n";
				std::cout << "Average for file " << filePath << " is:\n" << average << "\n\n";
				outputFile << "Median for file " << filePath << " is:\n" << median << "\n";
				outputFile << "Average for file " << filePath << " is:\n" << average << "\n\n";
			}
			catch (const std::runtime_error& e) {
				std::cerr << e.what() << std::endl;
				return 1;
			}
		}
		else {
			std::cout << "File " << filePath << " did not open successfully." << std::endl;
			return 1;
		}
	}
	return 0;
}