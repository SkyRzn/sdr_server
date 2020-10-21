set(CMAKE_INSTALL_PREFIX ${CONTAINER_PATH})


if (EXAMPLE)
	add_subdirectory(examples)
endif()

if (TEST_COVERAGE)
	add_subdirectory(test)
endif()
