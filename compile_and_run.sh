#!/bin/bash

# Check that exactly one argument is given
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <filename.kt>"
    exit 1
fi

# Extract filename and base name (without extension)
kt_file="$1"
base_name="${kt_file%.kt}"

# Step 1: clean and build
make clean
make

# Step 2: generate assembly
./k0 "$kt_file" -asm

# Step 3: compile with gcc
gcc "${base_name}.s" k0rt.c -o "$base_name"

echo "================ PROGRAM OUTPUT ================"
# Step 4: run executable
./"$base_name"