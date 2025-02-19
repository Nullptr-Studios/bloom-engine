/**
 * \file entrypoint.hpp
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief Entrypoint for our main function
 */

#pragma once
#include "engine.hpp"
#include "log.hpp"
#include <spdlog/spdlog.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#ifdef BLOOM_PLATFORM_WINDOWS

int main(int argc, char** argv) {
  const auto _engine = bloom::CreateEngine();

  glfwInit();
  _engine->Begin();

  while (!_engine->ShouldClose()) {
    _engine->Tick();
    _engine->Render();
  }

  _engine->End();
}

#endif