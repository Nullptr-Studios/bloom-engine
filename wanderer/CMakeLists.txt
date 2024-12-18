cmake_minimum_required(VERSION 3.29)
project(wanderer)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)
set(CMAKE_CXX_STANDARD 23)

add_executable(wanderer wanderer.cpp
        wanderer.hpp
        editor_camera.cpp
        editor_camera.hpp
)

target_link_libraries(wanderer PUBLIC bloom-engine)
