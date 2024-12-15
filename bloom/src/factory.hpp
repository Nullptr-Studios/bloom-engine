/**
 * @file factory.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 10/12/2024
 *
 * @brief [Brief description of the file's purpose]
 */

#pragma once
#include "objects/actor.hpp"
#include "objects/object.hpp"

namespace bloom {

class BLOOM_API Factory {
public:
  Factory () {}

  /**
   * Creates an Entity and does all the set-up processes that need to happen
   *
   * @tparam T Class to create an entity from
   * @return Created entity
   */
  template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
  std::shared_ptr<T> CreateObject();

  ObjectMap GetObjects() { return objects; }
  ActorMap GetRenderables() { return renderables; }

private:
  /**
   * This variable keeps track of the current object ID
   */
  id_t m_currentID = 0;

  ObjectMap objects;
  ActorMap renderables;
};

template<typename T, typename>
std::shared_ptr<T> Factory::CreateObject() {
  std::shared_ptr<T> obj = std::make_shared<T>(m_currentID);
  m_currentID++;

  objects.emplace(obj->GetID(), std::static_pointer_cast<Object>(obj));

  // Adding to the renderable list if it's an Actor
  if constexpr (std::is_base_of_v<Actor, T>) {
    renderables.emplace(obj->GetID(), std::static_pointer_cast<Actor>(obj));
  }

  BLOOM_INFO("Created object with ID: {0}", obj->GetID());
  return obj;
}

}
