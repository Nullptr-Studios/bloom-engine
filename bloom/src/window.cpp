#include "window.h"

#include "events/game_event.h"
#include "events/key_event.h"
#include "events/mouse_event.h"

namespace bloom {

Window::Window(int width, int height, std::string title)
  : _width(width), _height(height), _title(title) { }

Window::~Window() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}

void Window::OnInit() {
  _data.title = _title;
  _data.width = _width;
  _data.height = _height;
  SetVSync(true);

  glfwSetErrorCallback(GLFWErrorCallback);

  BLOOM_LOG("Creating window {0} ({1}, {2})", _title, _width, _height);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  _window = glfwCreateWindow(_width, _height, _title.c_str(), nullptr, nullptr);

  if(_window == nullptr) {
    BLOOM_CRITICAL("Failed to create GLFW window");
  }

  //xTODO: This is OpenGL specific
  //glfwMakeContextCurrent(_window);
  glfwSetWindowUserPointer(_window, &_data);
  BLOOM_INFO("Window created");

  #pragma region callbacks

  glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    data.width = width;
    data.height = height;
    WindowResizeEvent event(width, height);
    data.callback(event);
  });

  glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.callback(event);
  });

  glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    switch(action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key,0);
        data.callback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.callback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(key, 1);
        data.callback(event);
        break;
      }
    }
  });

  glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    switch(action) {
      case GLFW_PRESS: {
        MouseButtonPressedEvent event(button);
        data.callback(event);
        break;
      }
      case GLFW_RELEASE: {
        MouseButtonReleasedEvent event(button);
        data.callback(event);
        break;
      }
    }
  });

  glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    MouseScrolledEvent event((float)xOffset, (float)yOffset);
    data.callback(event);
  });

  glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xPos, double yPos) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    MouseMovedEvent event((float)xPos, (float)yPos);
    data.callback(event);
  });

#pragma endregion
}

void Window::OnTick() {
  glfwPollEvents();
  // xTODO: This is OpenGL specific
  //glfwSwapBuffers(_window);
}

void Window::SetVSync(bool enabled) {
  // This is OpenGL specific
  /* if(enabled) glfwSwapInterval(1);
  else glfwSwapInterval(0);

  _data.vsync = enabled;*/
}

void Window::GLFWErrorCallback(int error, const char *description) {
  BLOOM_ERROR("GLFW error ({0}): {1}", error, description);
}

bool Window::OnWindowExit() const { return glfwWindowShouldClose(_window); }

}
