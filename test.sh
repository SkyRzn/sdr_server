#!/bin/bash

build/test/sdr_server_test
lcov --capture --directory . --output-file test/coverage.info
genhtml test/coverage.info --output-directory test/html