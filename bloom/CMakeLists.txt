cmake_minimum_required(VERSION 3.29)
project(bloom_engine)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)
add_compile_definitions(BLOOM_BUILD_DLL)

set(VULKAN_SDK "C:/VulkanSDK/1.3.296.0")

add_library(bloom-engine SHARED
            bloom.h
            src/engine.h
            src/engine.cpp
            src/core.h
            src/entrypoint.h
            src/Log.h
            src/log.cpp
            src/events/event.h
            src/events/game_event.h
            src/events/key_event.h
            src/events/mouse_event.h
            src/events/key_event.h
        bloom_header.h
        src/window.cpp
        src/window.h
)

target_precompile_headers(bloom-engine PRIVATE bloom_header.h)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(bloom-engine PRIVATE -include bloom_header.h)
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
