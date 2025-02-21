#!/bin/sh
cmake -S .  -B build -DCMAKE_TOOLCHAIN_FILE=build/dependencies/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build build