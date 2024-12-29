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

int main() {
  const auto m_engine = bloom::CreateEngine();
  m_engine->Init();

  while (!m_engine->ShouldClose()) {
    m_engine->OnTick();
    m_engine->OnRender();
  }

  m_engine->OnClose();
}

#endif
