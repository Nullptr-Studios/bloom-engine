/**
 * @file object.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 10/12/2024
 *
 * @brief This file defines the base Object class and the Transform struct
 */

#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "src/render/devices.hpp"
#include <bloom_header.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace bloom {

/**
 *  @struct Transform
 *  @brief Represents the position, rotation, and scale of an object in 3D space.
 *
 *  The Transform struct provides a convenient way to store and manipulate the transformation
 *  properties of an object. It contains the object's @b position as a 3D vector, its @b scale
 *  as a 3D vector, and its @b rotation as Euler angles (yaw, pitch, and roll). The @c Matrix()
 *  method calculates the combined transformation matrix based on these properties.
 */
struct BLOOM_API Transform {
  glm::vec3 position = {}; ///< @brief The position of the object in world space.
  glm::vec3 scale = {1.0f, 1.0f, 1.0f}; ///< @brief The scale of the object in each axis.
  glm::vec3 rotation = {}; ///< @brief The rotation of the object as Euler angles (yaw, pitch, roll) in radians.

  /**
   *  @brief Calculates and returns the 4x4 transformation matrix.
   *
   *  This method combines the position, rotation, and scale properties into a single
   *  homogeneous matrix that can be used to transform the object's vertices.
   *  The transformation order is scale, then rotation, then translation.
   *
   *  @return The combined transformation matrix.
   */
  glm::mat4 Matrix() const {
    const glm::mat4 rotationMatrix = glm::yawPitchRoll(rotation.y, rotation.x, rotation.z);
    const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scale);
    const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), position);
    return translationMatrix * rotationMatrix * scaleMatrix;
  }
};

/**
 *  @class Object
 *  @brief Base class for all objects in the game world.
 *
 *  The Object class provides a common interface and basic functionality for all objects
 *  in the game. It manages the object's ID and provides virtual methods for the game loop
 *  stages: @c Begin, @c Tick, and @c End. Derived classes can override these methods to
 *  implement their specific behavior.
 */
class BLOOM_API Object {
public:
  /**
   *  @brief Constructs a new Object.
   *
   *  @param id The unique identifier for this object.
   *  @param devices A pointer to the Vulkan device management object.
   */
  explicit Object(const id_t id, render::Devices* devices) : m_devices(devices), m_id(id) {};
  virtual ~Object() = default;

  // Copy constructors
  Object(const Object&) = delete; 
  Object& operator=(const Object&) = delete; 
  Object(Object&&) = default; 
  Object& operator=(Object&&) = default;

  // Game loop
  /**
   *  @brief Called at the beginning of the scene.
   */
  virtual void Begin() {}
  /**
   *  @brief Called every frame to update the object's state.
   *  @param deltaTime The time elapsed since the last frame.
   */
  virtual void Tick(float deltaTime) {}
  /**
   *  @brief Called at the end of the scene.
   */
  virtual void End() {}

  Transform transform; ///< @brief The object's transform, representing its position, rotation, and scale.

  // Getters - Setters
  // ReSharper disable once CppInconsistentNaming
  /**
   *  @brief Retrieves the object's unique ID.
   *
   *  @return The object's ID.
   */
  id_t GetID() const { return m_id; }
protected:
  render::Devices* m_devices = nullptr; ///< @brief Pointer to the Vulkan device management object.
private:
  id_t m_id; ///< @brief The unique identifier for this object.
};

/**
 *  @typedef ObjectMap
 *  @brief A map of object IDs to shared pointers of Objects. Used for managing
 *         and accessing objects in the game world.
 */
typedef std::unordered_map<id_t,std::shared_ptr<Object>> ObjectMap;

}

#undef GLM_ENABLE_EXPERIMENTAL
