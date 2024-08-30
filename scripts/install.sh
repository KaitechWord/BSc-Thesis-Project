#!/bin/sh
conan profile detect --force
conan install . --output-folder=build/dependencies --build=missing -s build_type=Debug