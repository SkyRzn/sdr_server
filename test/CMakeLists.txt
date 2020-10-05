project(${PROJECT_NAME}_test)

file(GLOB_RECURSE SOURCES "test.c")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries (${PROJECT_NAME} ${LIB_NAME} pthread dl)
