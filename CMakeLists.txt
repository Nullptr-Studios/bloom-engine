cmake_minimum_required(VERSION 3.29)
project(bloom)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}_DIR}/bin/int)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall")

add_subdirectory(bloom)
add_subdirectory(sandbox)
add_subdirectory(wanderer)
