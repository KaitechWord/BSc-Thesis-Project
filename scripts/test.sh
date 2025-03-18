#!/bin/sh
# threads min/max naive/mix/smart datapath maskSize

data_path=./data/10xLakeMountain.jpg

echo $(date '+%Y-%m-%d %H:%M:%S\n') | tee -a TEST_RESULTS.txt
for threads_num in 1 2 3 4 5 6
do
 for approach in 0 1 2
 do
  for variant in 0 1
  do
   for mask_size in 5 15 25 35 45 55
   do
    ./bin/Image $threads_num $variant $approach $data_path $mask_size | tee -a TEST_RESULTS.txt
   done
  done
 done
done
echo '' | tee -a TEST_RESULTS.txt