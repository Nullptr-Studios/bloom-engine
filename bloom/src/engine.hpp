/**
 * \file engine.hpp
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief Main engine class
 */

#pragma once

#include "core.hpp"
#include "window.hpp"
#include "render/pipeline.hpp"
#include "render/devices.hpp"
#include "render/swap_chain.hpp"
#include "render/renderer.hpp"

namespace bloom {

class BLOOM_API Engine {

public:
  Engine();
  virtual ~Engine();

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;

  // Functions
  void Begin();
  void Tick();
  void Render();
  void End();

  inline bool ShouldClose() { return _window->ShouldClose(); }
  void OnEvent(Event& e);

protected:
  void CreatePipelineLayout();
  void CreatePipeline();
  void CreateCommandBuffers();
  void FreeCommandBuffers();
  void DrawFrame();
  void LoadModels();
  void RecreateSwapChain();
  void RecordCommandBuffer(int index);

  Window *_window = nullptr;
  std::unique_ptr<render::Devices> m_devices = nullptr;
  std::unique_ptr<render::SwapChain> m_swapChain = nullptr;
  std::unique_ptr<render::Pipeline> m_pipeline = nullptr;
  std::unique_ptr<render::Renderer> m_renderer = nullptr;
  VkPipelineLayout m_pipelineLayout;
  std::vector<VkCommandBuffer> m_commandBuffers;

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
