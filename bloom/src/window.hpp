/**
 * \file window.h
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
  bool OnWindowExit() const;

  inline void SetEventCallback(const EventCalbackFn& callback) { _data.callback = callback; };
  void SetVSync(bool enabled);
  inline bool IsVSync() const { return _data.vsync; };

  // Variables


private:
  GLFWwindow* _window;

  int _width;
  int _height;
  std::string _title;

  struct WindowData {
    std::string title;
    int width, height;
    bool vsync;
    EventCalbackFn callback;
  }_data;

  static void GLFWErrorCallback(int error, const char* description);
};

}
