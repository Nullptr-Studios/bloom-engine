/**
 * \file entrypoint.h
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#pragma once
#include "engine.h"
#include <spdlog/spdlog.h>

#ifdef BLOOM_PLATFORM_WINDOWS

int main(int argc, char** argv) {
  auto _engine = bloom::CreateEngine();
  spdlog::warn("Easy padding in numbers like {:08d}", 12);
  spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  _engine->Run();
  delete _engine;
}

#endif