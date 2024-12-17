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
  /**
   * Creates an Entity and does all the set-up processes that need to happen
   *
   * @tparam T Class to create an entity from
   * @return Created entity
   */
  template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
  std::shared_ptr<T> CreateObject();

  ObjectMap GetObjects() { return m_objects; }
  ActorMap GetRenderables() { return m_renderables; }

  void SetDevice(render::Devices* device) { m_devices = device; }

private:
  /**
   * This variable keeps track of the current object ID
   */
  id_t m_currentID = 0;

  ObjectMap m_objects;
  ActorMap m_renderables;

  render::Devices* m_devices;
};

template<typename T, typename>
std::shared_ptr<T> Factory::CreateObject() {
  std::shared_ptr<T> obj = std::make_shared<T>(m_currentID, m_devices);
  m_currentID++;

  // This crashes the program, it will work for now -x
  if (m_currentID == 0) {
    BLOOM_CRITICAL("Reached maximum number of objects");
  }

  m_objects.emplace(obj->GetID(), std::static_pointer_cast<Object>(obj));

  // Adding to the renderable list if it's an Actor
  if constexpr (std::is_base_of_v<Actor, T>) {
    m_renderables.emplace(obj->GetID(), std::static_pointer_cast<Actor>(obj));
  }

  BLOOM_INFO("Created object with ID: {0}", obj->GetID());
  return obj;
}

}
