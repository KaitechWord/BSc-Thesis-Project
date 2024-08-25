#!/bin/bash

# Define paths
exePath="./bin/Image"
deletePreviousRecordsPath="./output/*.txt"

# Check if the correct number of arguments is provided
if [ "$#" -lt 4 ]; then
    echo "Usage: $0 <number_of_repetitions> <number_of_threads> <mask_size> <file_path>"
    exit 1
fi

# Parse arguments
runCount=$1
threadsNum=$2
maskSize=$3
filePath=$4

minVariant=0
maxVariant=1

naiveApproach=0
smartApproach=1

# Check if the executable file exists
if [ ! -f "$exePath" ]; then
    echo "Error: Executable file '$exePath' not found."
    exit 1
fi

# Remove previous records
rm -f $deletePreviousRecordsPath

# Run the executable multiple times
for ((i=0; i<runCount; i++)); do
    echo "Running $exePath ($((i + 1))/$runCount)..."
    
    "$exePath" 1 $minVariant $naiveApproach "$filePath" $maskSize
    "$exePath" 1 $maxVariant $naiveApproach "$filePath" $maskSize
    "$exePath" $threadsNum $minVariant $naiveApproach "$filePath" $maskSize
    "$exePath" $threadsNum $maxVariant $naiveApproach "$filePath" $maskSize
    "$exePath" 1 $minVariant $smartApproach "$filePath" $maskSize
    "$exePath" 1 $maxVariant $smartApproach "$filePath" $maskSize
    "$exePath" $threadsNum $minVariant $smartApproach "$filePath" $maskSize
    "$exePath" $threadsNum $maxVariant $smartApproach "$filePath" $maskSize
done

# Run the Statistics executable
./bin/Statistics