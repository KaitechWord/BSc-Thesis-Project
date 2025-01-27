#include "NumCpp/Core/Slice.hpp"
#include "NumCpp/NdArray/NdArrayCore.hpp"
#include <NumCpp.hpp>
#include <array>
#include <chrono>

nc::NdArray<int> naive_v1(const std::vector<int> &data, int W) {
  unsigned output_size = data.size() - W + 1;

  // Create a stacked 2D array with W rows and output_size columns
  nc::NdArray<int> data_stacked(W, output_size);
  std::cout << data_stacked << std::endl;

  // std::cout << data( 0, nc::Slice(0, output_size));
  // data_stacked(0, nc::Slice(0, output_size)) = data(0, nc::Slice(0,
  // output_size)); std::cout << "test" << data_stacked;

  // Fill the stacked array with overlapping slices of the input
  for (int i = 0; i < W; ++i) {
    auto end = std::min(i + data.size() - W + 1, data.size() - 1);
    // Directly assign the slice to the i-th row of the stacked array
    // data_stacked.put(i, nc::Slice(i, end), data(i, nc::Slice(i, end)));
    // data_stacked.put(nc::Slice(i, end), data(i, end));
    std::cout << data_stacked << std::endl;
  }

  std::cout << data_stacked << std::endl;
  // Compute the minimum along the columns (axis 0)
  return nc::min(data_stacked, nc::Axis::ROW);
}

int main() {
  // Input array and window size
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(0, 255);
  std::vector<int> signal;
  for (auto i = 0; i < 10; ++i)
    signal.push_back(static_cast<uint8_t>(dist(gen)));
  nc::NdArray<int> data = signal;
  int W = 5;

  // Call the function

  std::cout << data;
  auto start = std::chrono::high_resolution_clock::now();
  nc::NdArray<int> output = naive_v1(signal, W);
  auto end = std::chrono::high_resolution_clock::now();
  const auto execTime =
      std::chrono::duration<double, std::milli>(end - start).count();
  std::cout << "Filter execution time : " << execTime << "ms.\n";
  std::cout << std::fixed
            << "Elem/sec:" << (data.size() / 1000000.0) / (execTime * 0.001)
            << std::endl;
  std::cout << output;

  // Print the result

  return 0;
}