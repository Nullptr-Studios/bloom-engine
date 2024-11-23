/**
 * \file window.h
 * \author Xein <xgonip@gmail.com>
 * \date 23/11/2024
 *
 * \brief File for window class
 */

#pragma once
#include "bloom_header.h"
#include "log.h"
#include <GLFW/glfw3.h>

namespace bloom {

class BLOOM_API Window {

public:
  Window(int width, int height, std::string title);
  ~Window();

  // Functions
  bool WindowShouldClose() const;

  // Variables


private:
  GLFWwindow* _window;

  int _width;
  int _height;
  std::string _title;
};

}
