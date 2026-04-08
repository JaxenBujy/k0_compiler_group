#!/bin/bash
make clean
make
./k0 foo.kt -tree -dot
dot -Tpng foo.kt_tree.dot > tree_img.png