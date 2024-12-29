cmake_minimum_required(VERSION 3.29)
project(sandbox)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)

add_executable(sandbox main.cpp)

add_dependencies(sandbox Shaders)

target_link_libraries(sandbox PUBLIC bloom-engine)

#copy res folder to build directory
add_custom_command(TARGET sandbox POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)