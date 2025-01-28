/**
 * @file engine.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 20/11/2024
 *
 * @brief Main engine class
 */

#pragma once
#include <bloom_header.hpp>
#include "layers/layer_stack.hpp"
#include "render/descriptor_layouts.hpp"
#include "render/renderer.hpp"

/**
 * @namespace bloom
 * @brief Contains the core classes and functions for the bloom engine.
 *
 * The @c bloom namespace encapsulates the main components and utilities required for the bloom engine.
 * This includes the main engine class, window management, factory for object creation, and core utilities.
 *
 * @details
 * The @c bloom namespace is designed to provide a comprehensive set of tools for managing the game loop,
 * rendering, and event handling. It includes classes for initializing and managing various subsystems
 * such as the window, renderer, and input handling. The namespace is structured to facilitate the creation,
 * management, and execution of a game using the bloom engine.
 *
 * Key Classes:
 * - @b Engine: Manages the main game loop, rendering, and event handling.
 * - @b Window: Manages the window and its events.
 * - @b Factory: Handles the creation and management of game objects.
 *
 * Example Usage:
 * @code
 * // Create a class that inherits from the Engine class (sandbox.hpp)
 * class Sandbox : public bloom::Engine {
 * public:
 *   Sandbox() = default;
 *
 *   void OnBegin() override;
 *   void Tick() override;
 *   void End() const override;
 * };
 *
 * // And then create an instance of the engine (sandbox.cpp) using the CreateEngine function
 * using namespace bloom;
 * std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::make_unique<sandbox::Sandbox>(); }
 * @endcode
 */
namespace bloom {
class Layer;
class Camera;
class Event;

namespace render {
class SimpleRenderSystem;
}

/**
* @class Engine
* @brief Main class for the bloom engine.
*
* The Engine class manages the main game loop, rendering, and event handling. It initializes
* and manages various subsystems such as the window, renderer, and input handling.
*/
class BLOOM_API Engine {
public:
  Engine();
  virtual ~Engine() = default;

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  // Game loop
  /**
   *  @brief Handles the initialization of the engine
   *  @note Do not access this in other classes
   */
  void Init();
  virtual void OnBegin() {} ///< Called before starting game loop
  virtual void OnTick();    ///< Called every frame
  virtual void OnRender();  ///< Called every frame after the @c Tick
  virtual void OnClose();   ///< Called before ending the game loop

  void OnEvent(Event & e);
  bool ShouldClose() const { return m_shouldClose; }

  /**
   * @brief Gets the DescriptorLayouts struct
   * @return The DescriptorLayouts struct
   */
  static render::DescriptorLayouts GetDescriptorLayouts() { return m_descriptorLayouts; }
  /**
   * @brief Sets the global descriptor set layout
   * @param layout The descriptor set layout
   */
  static void SetUBOLayout(render::DescriptorSetLayout* layout) { m_descriptorLayouts.globalLayout = layout; }
  /**
   * @brief Sets the material descriptor set layout
   * @param layout The descriptor set layout
   */
  static void SetMaterialLayout(render::DescriptorSetLayout* layout) { m_descriptorLayouts.materialLayout = layout; }

protected:
  Window* m_window = nullptr;
  std::unique_ptr<render::Devices> m_devices = nullptr;
  std::unique_ptr<render::Renderer> m_renderer = nullptr;

  std::shared_ptr<Camera> m_activeCamera = nullptr;

  float m_deltaTime = 0.016f;
  void PushLayer(Layer *layer);
  void PushOverlay(Layer *overlay);

private:
  bool ExitEngine(Event& e);
  bool m_shouldClose = false;

  LayerStack m_layerStack;
  static render::DescriptorLayouts m_descriptorLayouts;
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
