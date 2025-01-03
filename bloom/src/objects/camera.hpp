/**
 * @file camera.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 12/12/2024
 *
 * @brief This file contains the declaration for the Base Camera class
 */

#pragma once
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "object.hpp"
#include "src/events/game_event.hpp"

// thx windows.h I hate you -x
#undef near
#undef far

namespace bloom {

/**
 *  @class Camera
 *  @brief Represents a camera within the game world, providing a view and projection for rendering.
 *
 *  The Camera class inherits from Object and manages the view and projection matrices necessary
 *  for rendering the scene from a specific viewpoint. It supports both orthographic and
 *  perspective projections. The camera's position and orientation are used to calculate the
 *  view matrix, which transforms world coordinates into camera space. The projection matrix
 *  then transforms camera space coordinates into clip space for rendering.
 *
 *  @note Thanks Naiara for explaining this on MAT140
 */
class BLOOM_API Camera : public Object {
public:
  explicit Camera(id_t id) : Object(id) {}

  // Game loop
  void OnBegin() override;
  void OnTick(float deltaTime) override;
  void OnClose() override;

  void OnEvent(Event &e) override;
  void PropertiesPanel() override;

  /**
   *  @brief Sets an orthographic projection for the camera.
   *
   *  An orthographic projection is a type of projection that does not simulate perspective.
   *  Parallel lines remain parallel in the projected image. All parameters of the function describe
   *  the view planes in world space.
   *
   *  @param left The left edge of the projection volume.
   *  @param right The right edge of the projection volume.
   *  @param bottom The bottom edge of the projection volume.
   *  @param top The top edge of the projection volume.
   *  @param near The near clipping plane distance.
   *  @param far The far clipping plane distance.
   */
  void SetOrthographicProjection(float left, float right, float bottom, float top, float near, float far);
  /**
   *  @brief Sets a perspective projection for the camera.
   *
   *  A perspective projection simulates how objects appear smaller as they get further away,
   *  creating a sense of depth.
   */
  void SetPerspectiveProjection();

  // Getters - Setters
  /**
   * @brief Retrieves the camera's projection matrix.
   * @return The projection matrix.
   */
  const glm::mat4& GetProjection() const { return m_projectionMatrix; }
  /**
   * @brief Retrieves the camera's view matrix.
   * @return The view matrix.
   */
  const glm::mat4& GetView() const { return m_viewMatrix; }

private:
  /**
   * @brief Updates the camera's view matrix based on its position and orientation.
   *
   * This method is called on the cameras update loop to recalculate the view matrix based on
   * the transform of the game object.
   */
  void UpdateView();
  bool SetAspectRatio(WindowResizeEvent e);

  glm::mat4 m_projectionMatrix = glm::mat4(1.0f); ///< @brief The camera's projection matrix.
  glm::mat4 m_viewMatrix = glm::mat4(1.0f); ///< @brief The camera's view matrix.
  // TODO: Implement this with event window resize
  float m_aspect = 1.0f; // Width / Height
  float m_fov = 70; // Field of view
  float m_near = 0.1f; // Near clipping plane
  float m_far = 100.0f; // Far clipping plane
};

}
