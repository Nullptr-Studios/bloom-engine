/**
 * @file factory.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 10/12/2024
 *
 * @brief [Brief description of the file's purpose]
 * TODO: THIS NEEDS A REWRITE
 */

#pragma once
#include "objects/actor.hpp"
#include "objects/object.hpp"
#include "objects/point_light.hpp"
#include "objects/directional_light.hpp"

namespace bloom {

/**
 * @class Factory
 * @brief Manages the creation of game objects
 *
 * The Factory class is responsible for creating game objects, assigning unique IDs to them,
 * and maintaining maps of all created objects and renderable actors.
 */
class BLOOM_API Factory {
public:
  Factory() { m_instance = this; }
  ~Factory() { m_instance = nullptr; }

  // Copy constructor
  Factory &operator=(const Factory &) = delete;
  Factory(const Factory &) = delete;

  /**
   * Creates an Entity and does all the set-up processes that need to happen
   *
   * @tparam T Class to create an entity from
   * @return Created entity
   */
  template <typename T, typename = std::enable_if_t<std::is_base_of_v<Object, T>>>
  std::shared_ptr<T> CreateObject(const std::string& name = "Unnamed Object");

  /**
   * @brief Gets all objects
   * @return Map of all objects
   */
  ObjectMap GetObjects() { return m_objects; }
  /**
   * @brief Gets all renderable objects (@c Actor class)
   * @return Map of all actors
   */
  ActorMap GetRenderables() { return m_renderables; }
  /**
   * @brief Gets all point lights
   * @return Map of all point lights
   */
  PointLightMap GetPointLights() { return m_pointLights; }
  /**
   * @brief Gets the directional light
   * @return Directional light
   */
  std::shared_ptr<DirectionalLight> GetDirectionalLight() { return m_directionalLight; }
  /**
   * @brief Gets the instance of the Factory
   * @return The instance of the Factory
   */
  static Factory* GetInstance() {
    if (!m_instance) {
      BLOOM_ERROR("Tried to access Factory but it has not been created yet");
      return nullptr;
    }
    return m_instance;
  }
  /**
   * @brief Destroys all objects
   * Calls the OnClose method of all objects and clears the @c Objects and @c Renderables maps
   */
  void DestroyAllObjects() {
    for (auto &[id, object] : m_objects) {
      object->OnClose();
      object.reset();
    }
    m_objects.clear();
    m_renderables.clear();
  }
private:
  id_t m_currentID = 0;        ///< Keeps track of the current ID
  ObjectMap m_objects;         ///< Map of all objects
  ActorMap m_renderables;      ///< Map of all actors
  PointLightMap m_pointLights; ///< Map of all point lights
  std::shared_ptr<DirectionalLight> m_directionalLight = nullptr; ///< Directional light

  static Factory* m_instance;
};

template<typename T, typename>
std::shared_ptr<T> Factory::CreateObject(const std::string& name) {
  std::shared_ptr<T> obj = std::make_shared<T>(m_currentID);
  obj->SetName(name);
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

  if constexpr (std::is_base_of_v<PointLight, T>) {
    m_pointLights.emplace(obj->GetID(), std::static_pointer_cast<PointLight>(obj));
  }

  if constexpr (std::is_base_of_v<DirectionalLight, T>) {
    if (m_directionalLight != nullptr) {
      BLOOM_ERROR("Directional light {0} with name {1} already exists", m_directionalLight->GetID(), m_directionalLight->GetName());
      obj.clear();
      return nullptr;
    }
    m_directionalLight = std::static_pointer_cast<DirectionalLight>(obj);
  }

  BLOOM_INFO("Created object {0} with ID: {1}", name, obj->GetID());
  return obj;
}

}
