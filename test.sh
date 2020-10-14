#!/bin/bash

cd build &&
make clean &&
cmake .. -DCOVERAGE=ON &&
make &&
cd .. &&
build/test/sdr_server_test
lcov --capture --directory . --output-file build/test/coverage.info &&
genhtml build/test/coverage.info --output-directory build/test/html