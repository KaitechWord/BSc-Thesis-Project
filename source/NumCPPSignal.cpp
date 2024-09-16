#include <iostream>
#include <NumCpp.hpp>
#include <chrono>
#include <cassert>

nc::NdArray<uint8_t> create_data(int N) {
    nc::Shape shape(N);

    nc::NdArray<uint8_t> data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);
    for (auto i = 0; i < n; ++i)
        data.push_back(static_cast<uint8_t>(dist(gen)));
    return data;
}

nc::NdArray<uint8_t> naive_v1(const nc::NdArray<uint8_t>& data, int W) {
    std::vector<nc::NdArray<uint8_t>> data_stacked;
    for (int i = 0; i < W; ++i) {
        data_stacked.emplace_back(data[i, data.size() - W + 1]);
    }
    nc::NdArray<uint8_t> stacked = nc::vstack(data_stacked);
    return nc::min(stacked, nc::Axis::ROW);
}

nc::NdArray<uint8_t> naive_v2(const nc::NdArray<uint8_t>& data, int W) {
    int N = data.size();
    int output_size = N - W + 1;
    nc::NdArray<uint8_t> output(output_size);

    int BLOCK = 1000 * W - (W - 1);
    int STEP = BLOCK - W + 1;
    
    for (int j = 0; j < (N - BLOCK) / STEP * STEP; j += STEP) {
        std::vector<nc::NdArray<uint8_t>> data_stacked;
        for (int i = 0; i < W; ++i) {
            data_stacked.emplace_back(data[j + i, j + i + BLOCK]);
        }
        nc::NdArray<uint8_t> stacked = nc::vstack(data_stacked);
        output[j, j + BLOCK] = nc::min(stacked, nc::Axis::ROW).at(0);
    }

    std::vector<nc::NdArray<uint8_t>> data_stacked;
    int j = (N - BLOCK) / STEP * STEP;
    for (int i = 0; i < W; ++i) {
        data_stacked.emplace_back(data[j + i, N - W + 1]);
    }
    nc::NdArray<uint8_t> stacked = nc::vstack(data_stacked);
    output[j, output_size] = nc::min(stacked, nc::Axis::ROW).at(0);

    return output;
}

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

        nc::NdArray<uint8_t> data = create_data(N);

        for (int attempt = 0; attempt < 5; ++attempt) {
            double elapsed_time = measure_time(naive_v1, data, W);
            std::cout << "naive_v1, elapsed time = " << elapsed_time << " sec\n";
        }
        std::cout << "\n";
        nc::NdArray<uint8_t> output1 = naive_v1(data, W);

        for (int attempt = 0; attempt < 5; ++attempt) {
            double elapsed_time = measure_time(naive_v2, data, W);
            std::cout << "naive_v2, elapsed time = " << elapsed_time << " sec\n";
            nc::NdArray<uint8_t> output3 = naive_v2(data, W);
            assert(output1 == output3);
        }
        std::cout << "\n";
    }
    return 0;
}
