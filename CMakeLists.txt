cmake_minimum_required(VERSION 2.6)

project (sdr_server C)

set(VERSION_INFO_MAJOR_VERSION 0)
set(VERSION_INFO_MINOR_VERSION 1)
set(VERSION_INFO_PATCH_VERSION 1)

set(CMAKE_INSTALL_PREFIX /usr/bin)
set(INSTALL_LIB_PREFIX /usr/lib)
set(CONTAINER_PATH /usr/lib/${PROJECT_NAME}/containers)
set(LIB_NAME "_${PROJECT_NAME}")

add_definitions(-DVERSION_MAJOR=${VERSION_INFO_MAJOR_VERSION})
add_definitions(-DVERSION_MINOR=${VERSION_INFO_MINOR_VERSION})
add_definitions(-DVERSION_PATCH=${VERSION_INFO_PATCH_VERSION})
add_definitions(-DPROJECT_NAME=${PROJECT_NAME})
add_definitions(-DCONTAINER_PATH=${CONTAINER_PATH})

add_compile_options(
	"-Wall"
	"$<$<CONFIG:DEBUG>:-O0;-g3;-ggdb>"
	"$<$<CONFIG:RELEASE>:-O3>"
)

option(TEST_COVERAGE "Build with test coverage" OFF)
option(EXAMPLE "Build with examples" ON)

if (TEST)
	add_definitions(-DTEST_COVERAGE=1)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

include_directories("include")

file(GLOB_RECURSE LIB_SOURCES "src/lib/*.c")
add_library(${LIB_NAME} SHARED ${LIB_SOURCES})
target_link_libraries (${LIB_NAME} pthread dl)
install(TARGETS ${LIB_NAME} DESTINATION ${INSTALL_LIB_PREFIX})

file(GLOB EXE_SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${EXE_SOURCES})
target_link_libraries (${PROJECT_NAME} ${LIB_NAME} dl)
install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX})

add_subdirectory(containers)

if (TEST_COVERAGE)
	add_subdirectory(test)
endif()


