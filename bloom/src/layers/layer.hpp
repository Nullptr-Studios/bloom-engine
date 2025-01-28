/**
 * @file layer.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 19/12/2024
 *
 * @brief Base layer class definition
 */

#pragma once

namespace bloom {
  
namespace render {
struct FrameInfo;
}

class Event;

/**
 * @class Layer
 * @brief A Layer stores all the things that have to be rendered together
 * Layers are an abstraction to rendering and they form all the things that have to get rendered together.
 * Good examples of layers are, for example, a Gizmo layer, a UI layer, a Debug Tools layer...
 * The base layer of the engine is the Game Layer and it contains all the objects that are in the 3d game world
 */
class BLOOM_API Layer {
protected:
  explicit Layer(const std::string& name = "Unnamed Layer") : m_debugName(name) {}

public:
  virtual ~Layer() = default;
  virtual void OnAttach() = 0;              ///< @brief Called when the layer is attached to the layer stack
  virtual void OnBegin() = 0;               ///< @brief Called before starting the game loop
  virtual void OnTick(float deltaTime) = 0; ///< @brief Called every frame
  virtual void OnRender(render::FrameInfo frameInfo) = 0;       ///< @brief Called every frame after the @c Tick
  virtual void OnDetach() = 0;              ///< @brief Called before ending the game loop
  virtual void OnEvent(Event& e) = 0;       ///< @brief Called when an event is triggered

  const std::string& GetName() const { return m_debugName; }

private:
  std::string m_debugName;
};

}
