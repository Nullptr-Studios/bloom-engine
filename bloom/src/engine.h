/**
 * \file Engine.h
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#pragma once

#include <iostream>

#include "core.h"


namespace bloom {

class BLOOM_API Engine {

public:
  Engine() = default;
  virtual ~Engine() = default;

  // Functions
  void Run();

};

// To be defined on client
Engine* CreateEngine();

}
