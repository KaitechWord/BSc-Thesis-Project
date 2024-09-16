#!/bin/sh
conan profile detect --force
conan install . --output-folder=build/dependencies --build=missing -s compiler.cppstd=17 -s build_type=Debug