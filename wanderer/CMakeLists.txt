cmake_minimum_required(VERSION 3.29)
project(wanderer)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)

add_executable(wanderer wanderer.cpp
        wanderer.hpp
        editor_camera.cpp
        editor_camera.hpp
)

add_dependencies(wanderer Shaders)

target_link_libraries(wanderer PUBLIC bloom-engine)

#copy res folder to build directory
add_custom_command(TARGET wanderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
