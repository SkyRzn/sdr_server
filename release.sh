#!/bin/bash

cd build &&
make clean &&
cmake .. -DCMAKE_BUILD_TYPE=Release -DTEST_COVERAGE=OFF&&
make
cd ..