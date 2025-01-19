#include <NumCpp.hpp>

nc::NdArray<double> naive_v1(const nc::NdArray<double> &data, int W) {
  // Get the size of the output array
  int output_size = data.size() - W + 1;

  // Create a stacked 2D array to mimic the slicing behavior
  nc::NdArray<double> data_stacked(W, output_size);
  for (int i = 0; i < W; ++i) {
    // Extract the slice and reshape to a row vector
    auto row_slice = data[nc::Slice(i, i + output_size)];
    row_slice.reshape(1, output_size); // Reshape to (1, output_size)

    std::cout << "row index: " << i << std::endl;
    std::cout << "row: " << row_slice << std::endl;
    // Assign the reshaped row to the appropriate row in data_stacked
    data_stacked.put(nc::Slice(i, i + 1), nc::Slice(0, output_size), row_slice);
  }
  // Compute the minimum along the rows (axis = 0)
  return nc::min(data_stacked, nc::Axis::ROW);
}

int main() {
  // Input array and window size
  nc::NdArray<double> data = {1, 3, 2, 6, 4, 5};
  int W = 3;

  // Call the function
  nc::NdArray<double> output = naive_v1(data, W);

  // Print the result
  std::cout << "Output: " << output << std::endl;

  return 0;
}