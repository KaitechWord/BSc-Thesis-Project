import random
import sys
import time

import numpy as np


def create_data(N: int):
  return np.array(np.random.randint(256, size = N), dtype = np.uint8)


def naive_v1(data, W: int):
  print(data)
  data_stacked = tuple(data[i : i + len(data) - W + 1] for i in range(W))
  v = np.vstack(data_stacked)
  print(v)
  output = np.min(v, axis = 0)
  print(output)
  return output


def naive_v2(data, W: int):
  output = np.empty(len(data) - W + 1, dtype = data.dtype)

  BLOCK = 1_000 * W - (W - 1)
  assert BLOCK >= 2 * W + 1  
  
  STEP = BLOCK - W + 1
  
  for j in range(0, ((len(data) - BLOCK) // STEP) * STEP, STEP):
    data_stacked = tuple(data[j + i : j + i + BLOCK] for i in range(W))
    v = np.vstack(data_stacked)
    output[j : j + BLOCK] = np.min(v, axis = 0)
  
  # handling the last block
  j += STEP
  #last_block = len(data) - j - W + 1
  #data_stacked = tuple(data[j + i : j + i + last_block] for i in range(W))
  data_stacked = tuple(data[j + i : i + len(data) - W + 1] for i in range(W))
  v = np.vstack(data_stacked)
  output[j : ] = np.min(v, axis = 0)
  return output


# https://stackoverflow.com/questions/43288542/max-in-a-sliding-window-in-numpy-array
# https://stackoverflow.com/questions/40084931/taking-subarrays-from-numpy-array-with-given-stride-stepsize/40085052#40085052

def strided_app(a, L, S):  # Window len = L, Stride len/stepsize = S
    nrows = ((a.size - L) // S) + 1
    n = a.strides[0]
    return np.lib.stride_tricks.as_strided(a, shape = (nrows, L), strides = (S * n, n))


def min_filter1d_valid_strided(a, W):
    return strided_app(a, W, S = 1).min(axis = 1)


if __name__ == "__main__":
  N = 30_000_000
  #W = int(sys.argv[1])  # window size
  #assert N >= W
  print("Number of elements: ", N)
  
  for W in [5, 15, 25, 35, 45, 55]:
    print(f"W = {W}")
    print("-" * 10)
    
    data = create_data(N)
    
    for attempt in range(5):
      # print(data)
      t1 = time.time()
      output1 = naive_v1(data, W)
      t2 = time.time()
      # print(output1)
      print(f"naive_v1, elapsed time = {t2 - t1} sec, i.e., {round((len(data) / (t2 - t1)) / 1e6, 2):,} M elem / sec")
    print()

    for attempt in range(5):
      t1 = time.time()
      output2 = min_filter1d_valid_strided(data, W)
      t2 = time.time()
      print(f"min_filter1d_valid_strided, elapsed time = {t2 - t1} sec, i.e., {round((len(data) / (t2 - t1)) / 1e6, 2):,} M elem / sec") 
      assert np.all(output1 == output2)
    print()

    for attempt in range(5):
      t1 = time.time()
      output3 = naive_v2(data, W)
      t2 = time.time()
      print(f"naive_v3, elapsed time = {t2 - t1} sec, i.e., {round((len(data) / (t2 - t1)) / 1e6, 2):,} M elem / sec")
      assert np.all(output1 == output3)
    print()
  print()
