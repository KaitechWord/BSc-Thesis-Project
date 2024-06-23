cmake -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -G "Unix Makefiles" -S . -B .\build
Set-Location .\build
make
Set-Location ..