#include "window.hpp"

#include "events/game_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"

namespace bloom {

Window::Window(int width, int height, std::string title)
  : m_width(width), m_height(height), m_title(title) { }

Window::~Window() {
  glfwDestroyWindow(_window);
  glfwTerminate();
}

void Window::OnInit() {
  m_data.title = m_title;
  m_data.width = m_width;
  m_data.height = m_height;
  SetVSync(true);

  glfwSetErrorCallback(GLFWErrorCallback);

  BLOOM_LOG("Creating window {0} ({1}, {2})", m_title, m_width, m_height);

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  _window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(_window, this);
  glfwSetFramebufferSizeCallback(_window, FramebufferResizedCallback);

  if(_window == nullptr) {
    BLOOM_CRITICAL("Failed to create GLFW window");
  }

  // TODO: This is OpenGL specific
  //glfwMakeContextCurrent(_window);
  glfwSetWindowUserPointer(_window, &m_data);
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
  // TODO: This is OpenGL specific
  //glfwSwapBuffers(_window);
}

void Window::SetVSync(bool enabled) {
  // This is OpenGL specific
  /* if(enabled) glfwSwapInterval(1);
  else glfwSwapInterval(0);

  _data.vsync = enabled;*/
}

void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
  if(glfwCreateWindowSurface(instance, _window, nullptr, surface) != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to create window surface");
  }
}

void Window::FramebufferResizedCallback(GLFWwindow *window, int width, int height) {
  auto wd = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
  wd->m_framebufferResized = true;
  wd->SetDimensions(width, height);
}

void Window::SetDimensions(int width, int height) {
  m_width = width;
  m_height = height;
  m_data.width = width;
  m_data.height = height;
}

void Window::GLFWErrorCallback(int error, const char *description) {
  //BLOOM_ERROR("GLFW error ({0}): {1}", error, description);
}

void Window::CloseWindow() {
  glfwSetWindowShouldClose(_window, GLFW_TRUE);
}

}
