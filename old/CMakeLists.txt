cmake_minimum_required(VERSION 2.6)

project (sdr_server C)

set(VERSION_INFO_MAJOR_VERSION 0)
set(VERSION_INFO_MINOR_VERSION 1)
set(VERSION_INFO_PATCH_VERSION 1)

add_definitions(-DVERSION_MAJOR=${VERSION_INFO_MAJOR_VERSION})
add_definitions(-DVERSION_MINOR=${VERSION_INFO_MINOR_VERSION})
add_definitions(-DVERSION_PATCH=${VERSION_INFO_PATCH_VERSION})
add_definitions("-O3 -Wall")

file(GLOB_RECURSE SOURCES "src/*.c")
add_executable(sdr_server ${SOURCES})
target_link_libraries (sdr_server pthread)

file(GLOB_RECURSE TEST_SOURCES "test/*.c")

list(FILTER SOURCES EXCLUDE REGEX "^.*/main.c$")
add_executable(sdr_server_test ${TEST_SOURCES} ${SOURCES})
target_link_libraries (sdr_server_test pthread)


