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
		
project(Statistics
        VERSION 1.0
        LANGUAGES CXX)
		
project(NumCPPSignal
        VERSION 1.0
        LANGUAGES CXX)
		
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ROOT_DIR ${CMAKE_SOURCE_DIR})
add_definitions(-DROOT_DIR="${ROOT_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ROOT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ROOT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${ROOT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${ROOT_DIR}/bin)

include(source/CMakeLists.txt)
include(tools/CMakeLists.txt)