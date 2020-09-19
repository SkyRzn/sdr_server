cmake_minimum_required(VERSION 2.6)

project (sdr_server C)

set(VERSION_INFO_MAJOR_VERSION 0)
set(VERSION_INFO_MINOR_VERSION 1)
set(VERSION_INFO_PATCH_VERSION 1)

set(CMAKE_INSTALL_PREFIX /usr/bin)
set(CONTAINER_PATH /usr/lib/${PROJECT_NAME}/containers)

add_definitions(-DVERSION_MAJOR=${VERSION_INFO_MAJOR_VERSION})
add_definitions(-DVERSION_MINOR=${VERSION_INFO_MINOR_VERSION})
add_definitions(-DVERSION_PATCH=${VERSION_INFO_PATCH_VERSION})
add_definitions(-DPROJECT_NAME=${PROJECT_NAME})
add_definitions(-DCONTAINER_PATH=${CONTAINER_PATH})
add_definitions("-O3 -Wall")

file(GLOB_RECURSE SOURCES "src/*.c")

include_directories("include")

add_executable(${PROJECT_NAME} ${SOURCES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

target_link_libraries (${PROJECT_NAME} pthread dl)

add_subdirectory(containers)
