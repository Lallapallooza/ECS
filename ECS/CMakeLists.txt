cmake_minimum_required(VERSION 3.5.1)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../../lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../../bin)
set(project_name ECS)

project(${project_name})

file(GLOB SOURCES src/*.cpp)
file(GLOB INCLUDES include/*.h)

include_directories(include/ECS)

add_library(${project_name} SHARED ${SOURCES} ${INCLUDES})
add_library(${project_name} ${SOURCES} ${INCLUDES})