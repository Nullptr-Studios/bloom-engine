/**
 * @file layer.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 19/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "src/render/frame_info.hpp"

namespace bloom {

class BLOOM_API Layer {
protected:
  explicit Layer(const std::string& name = "Unnamed Layer") : m_debugName(name) {}

public:
  virtual ~Layer() = default;
  virtual void OnAttach() = 0;              ///< @brief Called when the layer is attached to the layer stack
  virtual void OnBegin() = 0;               ///< @brief Called before starting the game loop
  virtual void OnTick(float deltaTime) = 0; ///< @brief Called every frame
  virtual void OnRender(render::FrameInfo frameInfo) = 0;              ///< @brief Called every frame after the @c Tick
  virtual void OnDetach() = 0;              ///< @brief Called before ending the game loop
  virtual void OnEvent(const Event& e) = 0; ///< @brief Called when an event is triggered

  const std::string& GetName() const { return m_debugName; }

private:
  std::string m_debugName;
};

}
