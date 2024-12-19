cmake_minimum_required(VERSION 3.29)
project(bloom_engine)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)
add_compile_definitions(BLOOM_BUILD_DLL)
set(CMAKE_CXX_STANDARD 23)
set(VULKAN_SDK "C:/VulkanSDK/1.3.296.0")

add_library(bloom-engine SHARED
        bloom.hpp
        bloom_header.hpp
        src/render/descriptors.cpp
        src/render/descriptors.hpp
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
        src/render/pipeline.hpp
        src/render/pipeline.cpp
        src/render/devices.hpp
        src/render/devices.cpp
        src/render/swap_chain.hpp
        src/render/swap_chain.cpp
        src/render/model.hpp
        src/render/model.cpp
        src/render/renderer.hpp
        src/render/renderer.cpp
        src/objects/object.hpp
        src/factory.hpp
        src/simple_render_system.cpp
        src/simple_render_system.hpp
        src/render/texture.cpp
        src/render/texture.hpp
        src/objects/camera.cpp
        src/objects/camera.hpp
        src/objects/actor.cpp
        src/objects/actor.hpp
        src/render/buffer.cpp
        src/render/buffer.hpp
        src/render/frame_info.hpp
        src/layer.cpp
        src/layer.hpp
        src/layer_stack.cpp
        src/layer_stack.hpp
        src/layer_stack.cpp
        src/layer_stack.hpp
        src/factory.cpp
)

target_precompile_headers(bloom-engine PRIVATE bloom_header.hpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(bloom-engine PRIVATE -include bloom_header.hpp)
endif()

target_include_directories(bloom-engine PUBLIC
        lib/spdlog/include
        ${VULKAN_SDK}/Include
)

# GLM INCLUDE
add_subdirectory(lib/glm)
target_link_libraries(bloom-engine glm::glm)

# STB INCLUDE
target_include_directories(bloom-engine PUBLIC lib/stb)

# GLFW INCLUDE
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
target_link_libraries(bloom-engine glfw)

find_package(Vulkan REQUIRED)
target_link_libraries(bloom-engine Vulkan::Vulkan)

target_include_directories(bloom-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the output directory for the DLL
set(SANDBOX_DIR "${CMAKE_BINARY_DIR}/sandbox")
set(WANDERER_DIR "${CMAKE_BINARY_DIR}/wanderer")

# Add a post-build command to copy the DLL to the sandbox folder
add_custom_command(TARGET bloom-engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:bloom-engine>"
        "${SANDBOX_DIR}/$<TARGET_FILE_NAME:bloom-engine>"
)

# Add a post-build command to copy the DLL to the wanderer folder
add_custom_command(TARGET bloom-engine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:bloom-engine>"
        "${WANDERER_DIR}/$<TARGET_FILE_NAME:bloom-engine>"
)