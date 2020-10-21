cmake_minimum_required(VERSION 2.6)


add_library(common_test SHARED common.c)
install(TARGETS common_test DESTINATION ${CMAKE_INSTALL_PREFIX})


