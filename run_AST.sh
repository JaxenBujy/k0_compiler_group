#!/bin/bash
# runs with the AST option and automatically generates the png file of the tree
make clean
make
./k0 foo2.kt -tree -dot
dot -Tpng foo2.kt_tree.dot > tree_img.png