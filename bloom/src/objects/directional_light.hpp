/**
 * @file directional_light.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 06/01/2025
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "light.hpp"
#include "src/render/frame_info.hpp"

namespace bloom {

class BLOOM_API DirectionalLight : public Light {
public:
  explicit DirectionalLight(const id_t id) : Light(id) {};

  DirectionalLightInfo GetInfo();
private:
  const Type m_type = Directional;
};

}
