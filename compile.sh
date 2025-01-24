#!/bin/bash

# Set compiler and standard
CXX=g++
CXXFLAGS="-std=c++17 -Wall -Wextra"

# Source files
SRC_FILES=$(find . -name "*.cpp")

# Output file
OUTPUT="main"

# Compile command
$CXX $CXXFLAGS $SRC_FILES -o $OUTPUT

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful! Executable created: $OUTPUT"
else
    echo "Compilation failed."
fi
