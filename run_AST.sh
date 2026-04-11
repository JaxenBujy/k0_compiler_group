#!/bin/bash
make clean
make
./k0 sem22.kt -tree -dot
dot -Tpng sem22.kt_tree.dot > tree_img.png