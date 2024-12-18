/**
 * @file Wanderer.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 15/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include <bloom.hpp>
#include "editor_camera.hpp"

namespace wanderer {

class Wanderer : public bloom::Engine {
public:
  Wanderer() = default;

  void OnBegin() override;
  void OnTick() override;
  void OnClose() override;
private:
  std::shared_ptr<EditorCamera> m_camera;
};

}
