#!/bin/bash

cd build &&
make clean &&
cmake .. -DCMAKE_BUILD_TYPE=Release -DCOVERAGE=OFF&&
make
cd ..