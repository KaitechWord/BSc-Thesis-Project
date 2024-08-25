1. Libraries installed by conan:<br/>
* nlohmann_json (ver. 3.11.3 tested)<br/>
* opencv (ver. 4.9.0 tested)<br/>
* catch2 (ver. 3.7.0 tested)<br/>
2. data/Config.json file:
* ThreadsNumber(int) - number of threads that the filter will run with
* SignalFilter
* * Variant (string) - variant of algorithm -> "MIN" / "MAX"
* * Approach (string) - approach of algorithm -> "NAIVE" / "SMART"
* * DataPath (string) - path to txt file with signal input
* * MaskSize (int) - the size of mask
* ImageFilter
* * Variant (string) - variant of algorithm -> "MIN" / "MAX"
* * Approach (string) - approach of algorithm -> "NAIVE" / "SMART"
* * DataPath (string) - path to input file
* * MaskSize (int) - the size of one side of mask
3. Execute from root directory:<br/>
* to install dependencies: ./scripts/install.(sh/ps1)<br/>
* to build the project: ./scripts/build.(sh/ps1)<br/>
* to clean the project: ./scripts/clean.(sh/ps1)<br/>
* to run the project: ./scripts/run.(sh/ps1)<br/>
* * result image is saved in ./output/imageResult.png
* to run the project unit tests: ./scripts/runTests.(sh/ps1)<br/>
* to calculate the median/average execution time of all filters: 
* * ./scripts/calcStatistics.(sh/ps1) (number_of_repetitions) (number_of_threads) (mask_size) (file_path)
* * results are saved in ./output/statistics.txt
* * execution times can be viewed in other txts files in that directory
* It is possible to run project with custom arguments, but the correct way is to use config file, same rules as in 2. are applied:
* * ./bin/Image(.exe) (number_of_threads) (variant) (approach) (file_path) (mask_size)