/**
 * \file Engine.h
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief Main engine class
 */

#pragma once

#include "core.h"

namespace bloom {

class BLOOM_API Engine {

public:
  Engine() = default;
  virtual ~Engine() = default;

  // Functions
  /// \brief Core game loop function
  void Run();

};

/*
 *  This provides us with a way to create an entrypoint for the bloom engine
 *  Note for future xein: this is wanted as it makes the main function be handled by our engine
 *  rather than by our game
 *
 *  This function is declared here but implemented on main.cpp on the game project
 */
Engine* CreateEngine();

}
