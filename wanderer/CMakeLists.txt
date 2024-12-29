cmake_minimum_required(VERSION 3.29)
project(wanderer)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)

add_executable(wanderer wanderer.cpp
        wanderer.hpp
        editor_camera.cpp
        editor_camera.hpp
)

# IMGUI INCLUDE
set(IMGUI_DIR "../bloom/lib/imgui")
target_include_directories(bloom-engine PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
target_sources(bloom-engine PUBLIC
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

target_link_libraries(wanderer PUBLIC bloom-engine)

#copy res folder to build directory
add_custom_command(TARGET wanderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)
