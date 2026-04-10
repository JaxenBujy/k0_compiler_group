#!/bin/bash

OUTPUT_FILE="all_test_output.txt"

# Clear previous output
> "$OUTPUT_FILE"

make clean
make

COMPILER="./k0"
OPTION="-none"

echo "Running all tests..." | tee -a "$OUTPUT_FILE"
echo "==========================" | tee -a "$OUTPUT_FILE"

# Loop through desired test files
for file in $(ls tests/k0/lex*.kt tests/k0/syn*.kt tests/k0/sem*.kt tests/errors/lex*.kt tests/errors/syn*.kt tests/errors/sem*.kt tests/kotlin/lex*.kt tests/kotlin/syn*.kt tests/kotlin/sem*.kt 2>/dev/null | sort -V)
do
    if [ -f "$file" ]; then
        echo "" | tee -a "$OUTPUT_FILE"
        echo "===== Running: $file =====" | tee -a "$OUTPUT_FILE"

        # Run and capture BOTH stdout and stderr
        $COMPILER "$file" $OPTION >> "$OUTPUT_FILE" 2>&1
    fi
done

echo "" | tee -a "$OUTPUT_FILE"
echo "All tests completed." | tee -a "$OUTPUT_FILE"