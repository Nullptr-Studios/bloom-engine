/**
 * @file layer.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 19/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once

namespace bloom {

class Layer {
public:
  explicit Layer(const std::string& name = "Unnamed Layer") : m_debugName(name) {}
  virtual ~Layer() = default;

  virtual void OnAttach() = 0;
  virtual void OnDetach() = 0;
  virtual void OnTick() = 0;
  virtual void OnRender() = 0;
  virtual void OnEvent() = 0;

  const std::string& GetName() const { return m_debugName; }

private:
  std::string m_debugName;
};

}
