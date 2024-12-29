/**
 * \file window.hpp
 * \author Xein <xgonip@gmail.com>
 * \date 23/11/2024
 *
 * \brief File for window class
 */

#pragma once
#include "bloom_header.hpp"
#include "events/event.hpp"
#include <GLFW/glfw3.h>

namespace bloom {

/**
 * \class Window
 * \brief Manages the window and its events.
 *
 * The Window class is responsible for creating and managing the window, handling events,
 * and providing various window-related functionalities such as VSync and window resizing
 */
class BLOOM_API Window {

public:
  using EventCallbackFn = std::function<void(Event&)>; ///< Type alias for the event callback function

  Window(int width, int height, const std::string &title);
  ~Window();

  void OnBegin();
  void OnTick();

  void SetEventCallback(const EventCallbackFn& callback) { m_data.callback = callback; };
  VkExtent2D GetExtent() const { return {m_data.width, m_data.height}; }
  bool GetWindowResized() const { return m_framebufferResized; }
  static Window* GetInstance() { return m_instance; }
  GLFWwindow* GetWindow() const { return m_window; }
  double GetDeltaTime();

  /**
   * \brief Creates a Vulkan window surface.
   * \param instance The Vulkan instance.
   * \param surface The Vulkan surface to create.
   */
  void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface) const;
  /**
   * \brief Creates a Vulkan window surface.
   * \param instance The Vulkan instance.
   * \param surface The Vulkan surface to create.
   */
  void ResetWindowResized() { m_framebufferResized = false; }

private:
  static Window* m_instance;
  GLFWwindow* m_window = nullptr;
  static void FramebufferResizedCallback(GLFWwindow* window, int width, int height);
  static void GlfwErrorCallback(int error, const char* description);
  void SetDimensions(int width, int height);

  bool m_framebufferResized = false;

  /**
   * \struct WindowData
   * \brief Holds data related to the window.
   */
  struct WindowData {
    std::string title;          ///< The window title
    unsigned int width, height; ///< The window dimensions
    EventCallbackFn callback;   ///< The event callback function
  }m_data;
};

}
