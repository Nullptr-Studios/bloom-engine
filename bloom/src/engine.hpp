/**
 * @file engine.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 20/11/2024
 *
 * @brief Main engine class
 */

#pragma once
#include "core.hpp"
#include "window.hpp"
#include "factory.hpp"
#include "render/devices.hpp"
#include "render/renderer.hpp"
#include "simple_render_system.hpp"
#include "objects/camera.hpp"
#include <bloom_header.hpp>

namespace bloom {

class BLOOM_API Engine {
public:
  Engine() = default;

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  // Game loop
  void Begin();
  virtual void Tick();
  void Render();
  virtual void End() const;

  bool ShouldClose() const { return m_window->ShouldClose(); }
  void OnEvent(const Event & e) const;

  std::unique_ptr<Factory> factory = nullptr;

protected:
  virtual void OnBegin();

  Window* m_window = nullptr;
  std::unique_ptr<render::Devices> m_devices = nullptr;
  std::unique_ptr<render::Renderer> m_renderer = nullptr;
  SimpleRenderSystem* m_simpleRenderSystem = nullptr;

  std::shared_ptr<Camera> m_activeCamera;

  double m_deltaTime;
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
