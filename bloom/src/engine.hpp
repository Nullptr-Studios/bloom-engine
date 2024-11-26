/**
 * \file Engine.h
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief Main engine class
 */

#pragma once

#include "core.hpp"
#include "window.hpp"
#include "render/pipeline.hpp"

namespace bloom {

class BLOOM_API Engine {

public:
  Engine();
  virtual ~Engine() = default;

  // Functions
  void Begin();
  void Tick();
  void Render();
  void End();

  inline bool ShouldRun() { return true; }
  void OnEvent(Event& e);

protected:
  Window *_window = nullptr;
  render::Pipeline _pipeline{"resources/shaders/defaut.vert.spv", "resources/shaders/default.frag.spv"};

};

/**
 * @brief Creates an entry point for the Bloom engine.
 *
 * This function serves as the primary entry point for the engine, allowing it to handle
 * the main execution flow instead of the game directly. By abstracting the @c main function,
 * the engine can take full control over initialization, execution, and shutdown processes.
 *
 * @note For future Xein: This design is intentional to ensure that the engine manages
 * the @c main function rather than the game project. Any game-specific logic should
 * integrate with this entry point.
 *
 * @details While this function is declared here, its implementation resides in main.cpp
 * within the game project. This separation ensures modularity between the engine
 * and game layers.
 *
 * @returns A pointer to the @c Engine class instance, which is used to manage the engine's lifecycle.
 */
std::unique_ptr<bloom::Engine> CreateEngine();

}
