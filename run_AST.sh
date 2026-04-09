#!/bin/bash
make clean
make
./k0 hello_world.kt -tree -dot
dot -Tpng hello_world.kt_tree.dot > tree_img.png