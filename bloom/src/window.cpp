#include "window.h"

namespace bloom {

Window::Window(int width, int height, std::string title)
  : _width(width), _height(height), _title(std::move(title)) {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  _window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
  if(_window == nullptr) {
    BLOOM_CRITICAL("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(_window);
  BLOOM_INFO("Window created");
}

Window::~Window() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}

bool Window::WindowShouldClose() const { return glfwWindowShouldClose(_window); }

}
