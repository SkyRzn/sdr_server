cmake_minimum_required(VERSION 2.6)

project (sdr_server C)

set(VERSION_INFO_MAJOR_VERSION 0)
set(VERSION_INFO_MINOR_VERSION 1)
set(VERSION_INFO_PATCH_VERSION 1)

set(CMAKE_INSTALL_PREFIX /usr/bin)
set(CONTAINER_PATH /usr/lib/${PROJECT_NAME}/containers)
set(LIB_NAME "_${PROJECT_NAME}")

add_definitions(-DVERSION_MAJOR=${VERSION_INFO_MAJOR_VERSION})
add_definitions(-DVERSION_MINOR=${VERSION_INFO_MINOR_VERSION})
add_definitions(-DVERSION_PATCH=${VERSION_INFO_PATCH_VERSION})
add_definitions(-DPROJECT_NAME=${PROJECT_NAME})
add_definitions(-DCONTAINER_PATH=${CONTAINER_PATH})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -Wall --coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

file(GLOB_RECURSE LIB_SOURCES "src/*.c")

list(REMOVE_ITEM LIB_SOURCES "main.c")

include_directories("include")

add_library(${LIB_NAME} ${LIB_SOURCES})

add_executable(${PROJECT_NAME} "src/main.c")

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

target_link_libraries (${PROJECT_NAME} ${LIB_NAME} pthread dl)

add_subdirectory(containers)

add_subdirectory(test)

enable_testing()

add_test(NAME test COMMAND test/${PROJECT_NAME}_test)
