/**
 * \file entrypoint.h
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief Entrypoint for our main function
 */

#pragma once
#include <spdlog/spdlog.h>
#include "engine.h"
#include "log.h"

#ifdef BLOOM_PLATFORM_WINDOWS

int main(int argc, char** argv) {
  bloom::Log::Init();
  GAME_CRITICAL("Test info");
  BLOOM_ERROR("Test error");
  BLOOM_WARN("Test warning");
  BLOOM_INFO("Test info");
  BLOOM_LOG("Test trace");

  auto _engine = bloom::CreateEngine();
  _engine->Run();
  delete _engine;
}

#endif