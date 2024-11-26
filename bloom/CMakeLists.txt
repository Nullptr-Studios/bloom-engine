cmake_minimum_required(VERSION 3.29)
project(bloom_engine)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)
add_compile_definitions(BLOOM_BUILD_DLL)
set(CMAKE_CXX_STANDARD 23)
set(VULKAN_SDK "C:/VulkanSDK/1.3.296.0")

add_library(bloom-engine SHARED
            bloom.hpp
            bloom_header.hpp
            src/engine.hpp
            src/engine.cpp
            src/core.hpp
            src/entrypoint.hpp
            src/log.hpp
            src/log.cpp
            src/window.hpp
            src/window.cpp
            src/events/event.hpp
            src/events/game_event.hpp
            src/events/key_event.hpp
            src/events/mouse_event.hpp
            src/events/key_event.hpp
            src/render/pipeline.hpp 
            src/render/pipeline.cpp
)

target_precompile_headers(bloom-engine PRIVATE bloom_header.hpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(bloom-engine PRIVATE -include bloom_header.hpp)
endif()

target_include_directories(bloom-engine PUBLIC
        lib/spdlog/include
        ${VULKAN_SDK}/Include
)

# GLFW INCLUDE
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
target_link_libraries(bloom-engine glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(bloom-engine OpenGL::GL)

target_include_directories(bloom-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
