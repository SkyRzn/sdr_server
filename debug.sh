#!/bin/bash

cd build &&
make clean &&
cmake .. -DCMAKE_BUILD_TYPE=Debug -DTEST_COVERAGE=OFF&&
make
cd ..
