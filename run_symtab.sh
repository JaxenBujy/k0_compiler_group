#!/bin/bash
# runs with the -symtab option, printing the symbol table
make clean
make
./k0 hello_world.kt -symtab