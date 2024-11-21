cmake_minimum_required(VERSION 3.29)
project(bloom_engine)

add_compile_definitions(BLOOM_PLATFORM_WINDOWS)
add_compile_definitions(BLOOM_BUILD_DLL)

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
)

target_include_directories(bloom-engine PUBLIC lib/spdlog/include)
target_include_directories(bloom-engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
