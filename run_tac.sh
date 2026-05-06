#!/bin/bash
# runs with the -tac option, printing the intermediate code to the terminal
make clean
make
./k0 foo2.kt -tac