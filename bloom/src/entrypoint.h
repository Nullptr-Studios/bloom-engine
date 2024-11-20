/**
 * \file entrypoint.h
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#pragma once
#include "engine.h"

#ifdef BLOOM_PLATFORM_WINDOWS

int main(int argc, char** argv) {
  auto _engine = bloom::CreateEngine();
  _engine->Run();
  delete _engine;
}

#endif