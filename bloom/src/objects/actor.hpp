/**
 * @file actor.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 15/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "object.hpp"

namespace bloom {

class BLOOM_API Actor : public Object{

public:
  Actor(id_t id) : Object(id) {};
  ~Actor() = default;
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;
  Actor(Actor&&) = default;
  Actor& operator=(Actor&&) = default;

  void Begin() override;
  void Tick(float deltaTime) override;
  void End() override;

  std::shared_ptr<render::Model> model;
  glm::vec3 tintColor;
  render::Texture* texture;
private:
        
};

typedef std::unordered_map<id_t,std::shared_ptr<Actor>> ActorMap;

}
