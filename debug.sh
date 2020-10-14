#!/bin/bash

cd build &&
make clean &&
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=OFF&&
make
cd ..