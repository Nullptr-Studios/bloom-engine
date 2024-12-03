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

class BLOOM_API Window {

public:
  using EventCalbackFn = std::function<void(Event&)>;

  Window(int width, int height, std::string title);
  ~Window();

  // Functions
  void OnInit();
  void OnTick();
  void CloseWindow();

  inline void SetEventCallback(const EventCalbackFn& callback) { m_data.callback = callback; };
  void SetVSync(bool enabled);
  inline bool IsVSync() const { return m_data.vsync; };
  inline bool ShouldClose() const { return glfwWindowShouldClose(_window); };
  VkExtent2D GetExtent() {return {static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height)}; }

  void CreateWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

  bool GetWindowResized() { return m_framebufferResized; }
  void ResetWindowResized() { m_framebufferResized = false; }


  // Variables


private:
  GLFWwindow* _window;
  static void FramebufferResizedCallback(GLFWwindow* window, int width, int height);
  void SetDimensions(int width, int height);

  int m_width;
  int m_height;
  bool m_framebufferResized = false;
  std::string m_title;

  struct WindowData {
    std::string title;
    int width, height;
    bool vsync;
    EventCalbackFn callback;
  }m_data;

  static void GLFWErrorCallback(int error, const char* description);
};

}
