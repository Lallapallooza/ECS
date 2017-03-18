cmake_minimum_required(VERSION 3.5.1)


set(project_name ECS)

project(${project_name})

file(GLOB SOURCES src/*.cpp)
file(GLOB INCLUDES include/*.h)

include_directories(include/)

add_library(${project_name} SHARED ${SOURCES} ${INCLUDES})