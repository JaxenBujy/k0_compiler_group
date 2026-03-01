#!/bin/bash
make clean
make
valgrind ./k0 hello_world.kt