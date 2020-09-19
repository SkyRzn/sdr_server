cmake_minimum_required(VERSION 2.6)

project (skeleton C)

file(GLOB_RECURSE SOURCES "*.c")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})


