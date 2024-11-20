cmake_minimum_required(VERSION 3.29)
project(sandbox)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)

add_executable(sandbox main.cpp)

target_link_libraries(sandbox PUBLIC bloom-engine)
