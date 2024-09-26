#include <iostream>
#include <NumCpp.hpp>
#include <chrono>
#include <cassert>

// Function to create data (using int instead of uint8_t)
nc::NdArray<int> create_data(int N) {
    // Create a shape for the array (1D array of length N)
    nc::Shape shape(N);

    // Generate random integers in the range [0, 256) with int type
    return nc::random::randInt<int>(shape, 0, 256);  // upper bound is exclusive
}

// Naive version 1 (similar to naive_v1 in Python)
nc::NdArray<int> naive_v1(const nc::NdArray<int>& data, int W) {
    int N = data.size();
    nc::NdArray<int> output(N - W + 1);

    for (int i = 0; i <= N - W; ++i) {
        nc::NdArray<int> window = data[i, i + W];  // Extract subarray
        output[i] = nc::min(window);
    }

    return output;
}

// Naive version 2 (similar to naive_v2 in Python)
nc::NdArray<int> naive_v2(const nc::NdArray<int>& data, int W) {
    int N = data.size();
    int output_size = N - W + 1;
    nc::NdArray<int> output(output_size);

    int BLOCK = 1000 * W - (W - 1);
    int STEP = BLOCK - W + 1;

    for (int j = 0; j <= N - BLOCK; j += STEP) {
        nc::NdArray<int> block(BLOCK);
        for (int i = 0; i < W; ++i) {
            nc::NdArray<int> slice = data[j + i, j + i + BLOCK];
            block = nc::minimum(block, slice);
        }
        output[j, j + BLOCK] = block;
    }

    // Handle the last block
    int j = ((N - BLOCK) / STEP) * STEP;
    nc::NdArray<int> block(output_size - j);
    for (int i = 0; i < W; ++i) {
        nc::NdArray<int> slice = data[j + i, N - W + 1];
        block = nc::minimum(block, slice);
    }
    output[j, output_size] = block;

    return output;
}

// Using stride tricks (similar to strided_app and min_filter1d_valid_strided in Python)
nc::NdArray<int> min_filter1d_valid_strided(const nc::NdArray<int>& data, int W) {
    int nrows = (data.size() - W) + 1;
    nc::NdArray<int> output(nrows);

    for (int i = 0; i < nrows; ++i) {
        nc::NdArray<int> window = data[i, i + W];
        output[i] = nc::min(window);
    }

    return output;
}

// Helper function to measure execution time
template <typename F, typename... Args>
double measure_time(F func, Args&&... args) {
    auto start = std::chrono::high_resolution_clock::now();
    func(std::forward<Args>(args)...);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

int main() {
    const int N = 10'000'000;

    for (int W : {5, 15, 25, 35, 45}) {
        std::cout << "W = " << W << "\n";
        std::cout << "----------\n";

        // Create data
        nc::NdArray<int> data = create_data(N);

        // Test naive_v1
        for (int attempt = 0; attempt < 5; ++attempt) {
            double elapsed_time = measure_time([&]() { naive_v1(data, W); });
            std::cout << "naive_v1, elapsed time = " << elapsed_time << " sec\n";
        }
        std::cout << "\n";

        // Test min_filter1d_valid_strided
        nc::NdArray<int> output1 = naive_v1(data, W);
        for (int attempt = 0; attempt < 5; ++attempt) {
            double elapsed_time = measure_time([&]() { min_filter1d_valid_strided(data, W); });
            std::cout << "min_filter1d_valid_strided, elapsed time = " << elapsed_time << " sec\n";
            nc::NdArray<int> output2 = min_filter1d_valid_strided(data, W);
            assert(output1 == output2);
        }
        std::cout << "\n";

        // Test naive_v2
        for (int attempt = 0; attempt < 5; ++attempt) {
            double elapsed_time = measure_time([&]() { naive_v2(data, W); });
            std::cout << "naive_v2, elapsed time = " << elapsed_time << " sec\n";
            nc::NdArray<int> output3 = naive_v2(data, W);
            assert(output1 == output3);
        }
        std::cout << "\n";
    }
    return 0;
}