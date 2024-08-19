cmake_minimum_required(VERSION 3.22)

project(Signal
        VERSION 1.0
        LANGUAGES CXX)

project(Image
        VERSION 1.0
        LANGUAGES CXX)
        
project(ImageTests
        VERSION 1.0
        LANGUAGES CXX)

enable_testing()

include(source/CMakeLists.txt)