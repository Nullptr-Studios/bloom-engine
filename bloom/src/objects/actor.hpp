/**
 * @file actor.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 15/12/2024
 *
 * @brief This file contains the declaration of the actor class
 */

#pragma once
/**
 *  @def ALBEDO_PLACEHOLDER
 *  @brief Filepath for a purple checked missing texture in the Source Engine fashion
 */
#define ALBEDO_PLACEHOLDER "resources/textures/T_Default.tga"
/**
 *  @def RMO_PLACEHOLDER
 *  @brief Filepath for a simple RMO texture that is used as fallback if no texture is sent
 *
 *  This texture has a roughness of 0.5f, a metalic of 0.0f and an Ambient Occlusion of 1.0f
 *  This will be the default PBR settings for every material
 */
#define RMO_PLACEHOLDER "resources/textures/T_Default_RMO.tga"
/**
 *  @def NORMAL_PLACEHOLDER
 *  @brief Filepath for a fallback normal texture
 */
#define NORMAL_PLACEHOLDER "resources/textures/T_Default_N.tga"
/**
 *  @def EMISSION_PLACEHOLDER
 *  @brief Filepath for a fallback emission texture
 *
 *  This is a completely black image, meaning no emission by default
 */
#define EMISSION_PLACEHOLDER "resources/textures/T_Default_E.tga"

#include "object.hpp"
#include "src/render/descriptors.hpp"
#include "src/render/model.hpp"
#include "src/render/texture.hpp"

namespace bloom {

/**
 *  @class Actor
 *  @brief Represents a renderable and potentially interactive object within the game world.
 *
 *  The Actor class extends the Object class, adding rendering capabilities.  It serves as the base
 *  class for any object in the game that needs to be visually represented in the world. Actors manage
 *  their own textures, materials, and rendering properties.  They participate in the game's update loop
 *  through the @c Begin, @c Tick, and @c End methods, allowing for dynamic behavior and animation.
 */
class BLOOM_API Actor : public Object{
// TODO: This should have a protected constructor later on -x
public:
  /**
   * @brief Constructs a new Actor object.
   *
   * This constructor initializes a new Actor instance with a unique identifier and
   * a reference to the Vulkan rendering devices. It sets up the necessary resources
   * for managing textures and materials.
   *
   * @param id The unique identifier for this actor. This ID should be unique within
   *           the game world to avoid conflicts.
   */
  explicit Actor(id_t id);
  ~Actor() override = default;

  // Copy destructors
  Actor(const Actor&) = delete;
  Actor& operator=(const Actor&) = delete;
  Actor(Actor&&) = default;
  Actor& operator=(Actor&&) = default;

  /**
   * Loads textures for the actor's visual representation.
   *
   * This method loads the textures used to render the actor.  It supports loading
   * albedo, roughness-metallic-occlusion (RMO), normal, and emission textures.  If
   * no paths are provided for the textures, placeholder textures will be used instead.
   *
   * @param descriptorSetLayout The descriptor set layout for the textures. This layout
   *                           defines how the texture data will be accessed by the shaders.
   * @param albedo The path to the albedo texture file. The albedo texture defines the
   *               base color of the actor's surface.
   * @param rmo The path to the roughness-metallic-occlusion (RMO) texture file. This
   *            texture controls the surface properties of the actor, including how rough
   *            or smooth it appears and how metallic it is.
   * @param normal The path to the normal map texture file. The normal map modifies the
   *               perceived surface normals of the actor, adding detail and depth without
   *               increasing the polygon count.
   * @param emission The path to the emission texture file. The emission texture defines
   *                 areas of the actor that emit light, creating a glowing effect.
   */
  void LoadTextures(
    render::DescriptorSetLayout* descriptorSetLayout,
    const std::string& albedo = ALBEDO_PLACEHOLDER,
    const std::string& rmo = RMO_PLACEHOLDER,
    const std::string& normal = NORMAL_PLACEHOLDER,
    const std::string& emission = EMISSION_PLACEHOLDER
  );

  // Getters - Setters
  /**
   * Retrieves the descriptor set for the actor's textures.
   *
   * The descriptor set is a Vulkan object that contains information about the textures
   * used by the actor. This information is used by the shaders to access the texture
   * data during rendering.
   *
   * @return The Vulkan descriptor set for the actor's textures.
   */
  VkDescriptorSet GetDescriptorSet() const { return m_textureDescriptorSet; }

  /**
   * @brief The 3D model used to render the actor.  This model defines the
   *        shape and geometry of the actor.
   */
  std::shared_ptr<render::Model> model;
  /**
   * @brief A color tint applied to the actor's overall appearance. This color
   *        is passed on the push constant rather than on an UBO.
   */
  glm::vec3 tintColor = glm::vec3(1.0f);;
private:
  /**
   * @brief An array of textures used by the actor, including:
   *        - @b Albedo: The base color of the actor's surface.
   *        - @b RMO: Roughness, Metallic, and Ambient Occlusion properties of the surface.
   *        - @b Normal: Modifies the perceived surface normals for added detail.
   *        - @b Emission: Defines areas of the actor that emit light.
   */
  std::array<std::unique_ptr<render::Texture>, 4> m_textures;
  std::unique_ptr<render::DescriptorPool> m_materialPool;
  VkDescriptorSet m_textureDescriptorSet = VK_NULL_HANDLE;
};

/**
 *  @typedef ActorMap
 *
 *  A map of Actor IDs to shared pointers of Actors. This map is used to store and manage
 *  all active actors in the game world. The ID is used as the key for efficient lookup
 *  and retrieval of individual actors.
 */
typedef std::unordered_map<id_t,std::shared_ptr<Actor>> ActorMap;

}

#undef ALBEDO_PLACEHOLDER
#undef RMO_PLACEHOLDER
#undef NORMAL_PLACEHOLDER
#undef EMISSION_PLACEHOLDER