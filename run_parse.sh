#!/bin/bash
make clean
make
valgrind --leak-check=full ./k0 hello_world.kt