#include "window.hpp"
#include "events/game_event.hpp"
#include "events/key_event.hpp"
#include "events/mouse_event.hpp"

namespace bloom {

Window* Window::m_instance = nullptr;

Window::Window(int width, int height, const std::string& title) {
  m_data.width = width;
  m_data.height = height;
  m_data.title = title;
  m_instance = this;

  glfwInit();
}

Window::~Window() {
  glfwDestroyWindow(m_window);
  glfwTerminate();
}

void Window::OnBegin() {
  glfwSetErrorCallback(GlfwErrorCallback);

  BLOOM_LOG("Creating window {0} ({1}, {2})", m_data.title, m_data.width, m_data.height);
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
  m_window = glfwCreateWindow(m_data.width, m_data.height, m_data.title.c_str(), nullptr, nullptr);
  glfwSetWindowUserPointer(m_window, this);
  glfwSetFramebufferSizeCallback(m_window, FramebufferResizedCallback);

  if(m_window == nullptr) {
    BLOOM_CRITICAL("Failed to create GLFW window");
  }

  glfwSetWindowUserPointer(m_window, &m_data);
  BLOOM_INFO("Window created");

  // region Callbacks
  glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
    WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    data.width = width;
    data.height = height;
    WindowResizeEvent event(width, height);
    data.callback(event);
  });

  glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
    WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    WindowCloseEvent event;
    data.callback(event);
  });

  glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
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
      default:
        break;
    }
  });

  glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode) {
    WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
    KeyTypedEvent event(static_cast<int>(keycode));
    data.callback(event);
  });

  glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
    WindowData& data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
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
      default:
        break;
    }
  });

  glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
    WindowData& data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
    data.callback(event);
  });

  glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
    WindowData& data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
    MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
    data.callback(event);
  });
}

// region Tick
void Window::OnTick() {
  glfwPollEvents();
}

double Window::GetDeltaTime() {
  auto currentFrame = glfwGetTime();
  glfwSetTime(0);
  return currentFrame;
}

void Window::CreateWindowSurface(VkInstance instance, VkSurfaceKHR *surface) const {
  if(glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to create window surface");
  }
}

void Window::FramebufferResizedCallback(GLFWwindow *window, int width, int height) {
  const auto wd = static_cast<Window*>(glfwGetWindowUserPointer(window));
  wd->m_framebufferResized = true;
  wd->SetDimensions(width, height);
}

void Window::SetDimensions(const int width, const int height) {
  m_data.width = width;
  m_data.height = height;
}

void Window::GlfwErrorCallback(int error, const char *description) {
  BLOOM_ERROR("GLFW error ({0}): {1}", error, description);
}

}
