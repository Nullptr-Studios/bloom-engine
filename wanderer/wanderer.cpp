#include "wanderer.hpp"
#include "editor_camera.hpp"

using namespace bloom;
namespace wanderer {

std::unique_ptr<bloom::render::Model> createCubeModel(bloom::render::Devices* device, glm::vec3 offset) {
  std::vector<bloom::render::Model::Vertex> vertices = {
      {{-.5f, -.5f, -.5f}, {0.0f, 0.0f}, {.9f, .9f, .9f, 1.0f}},
      {{-.5f, .5f, .5f}, {1.0f, 1.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},
      {{-.5f, -.5f, .5f}, {0.0f, 1.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},
      {{-.5f, -.5f, -.5f}, {0.0f, 0.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},
      {{-.5f, .5f, -.5f}, {1.0f, 0.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},
      {{-.5f, .5f, .5f}, {1.0f, 1.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},

      // right face (yellow)
      {{.5f, -.5f, -.5f}, {0.0f, 0.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
      {{.5f, .5f, .5f}, {1.0f, 1.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
      {{.5f, -.5f, .5f}, {0.0f, 1.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
      {{.5f, -.5f, -.5f}, {0.0f, 0.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
      {{.5f, .5f, -.5f}, {1.0f, 0.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
      {{.5f, .5f, .5f}, {1.0f, 1.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},

      // top face (orange, remember y-axis points down)
      {{-.5f, -.5f, -.5f}, {0.0f, 0.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
      {{.5f, -.5f, .5f}, {1.0f, 1.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
      {{-.5f, -.5f, .5f}, {0.0f, 1.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
      {{-.5f, -.5f, -.5f}, {0.0f, 0.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
      {{.5f, -.5f, -.5f}, {1.0f, 0.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
      {{.5f, -.5f, .5f}, {1.0f, 1.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},

      // bottom face (red)
      {{-.5f, .5f, -.5f}, {0.0f, 0.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
      {{.5f, .5f, .5f}, {1.0f, 1.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
      {{-.5f, .5f, .5f}, {0.0f, 1.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
      {{-.5f, .5f, -.5f}, {0.0f, 0.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
      {{.5f, .5f, -.5f}, {1.0f, 0.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
      {{.5f, .5f, .5f}, {1.0f, 1.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},

      // nose face (blue)
      {{-.5f, -.5f, 0.5f}, {0.0f, 0.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},
      {{.5f, .5f, 0.5f},  {1.0f, 1.0f},{0.1f, 0.1f, 0.8f, 1.0f}},
      {{-.5f, .5f, 0.5f}, {0.0f, 1.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},
      {{-.5f, -.5f, 0.5f}, {0.0f, 0.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},
      {{.5f, -.5f, 0.5f}, {1.0f, 0.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},
      {{.5f, .5f, 0.5f}, {1.0f, 1.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},

      // tail face (green)
      {{-.5f, -.5f, -0.5f}, {0.0f, 0.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},
      {{.5f, .5f, -0.5f}, {1.0f, 1.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},
      {{-.5f, .5f, -0.5f}, {0.0f, 1.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},
      {{-.5f, -.5f, -0.5f}, {0.0f, 0.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},
      {{.5f, -.5f, -0.5f}, {1.0f, 0.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},
      {{.5f, .5f, -0.5f}, {1.0f, 1.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},

  };
  for (auto& v : vertices) {
    v.position += offset;
  }
  return std::make_unique<bloom::render::Model>(device, vertices);
}

// TODO: This needs a different name
void Wanderer::OnBegin() {
  m_camera = factory->CreateObject<EditorCamera>();
  m_activeCamera = m_camera;
  m_window->SetEventCallback([this](const Event& event) {
    m_camera->OnEvent(event);
  });

  std::shared_ptr<render::Model> model = createCubeModel(m_devices.get(), {0.0f, 0.0f, 0.0f});

  auto cube = factory->CreateObject<Actor>();
  cube->model = model;
  cube->transform.position = {-0.5f, 0.0f, -2.5f};
  cube->transform.scale = {0.5f, 0.5f, 0.5f};

  auto cube2 = factory->CreateObject<Actor>();
  cube2->model = model;
  cube2->transform.position = {0.5f, 0.0f, -2.5f};
  cube2->transform.scale = {0.5f, 0.5f, 0.5f};
}

void Wanderer::Tick() {
  Engine::Tick();
}

void Wanderer::End() const {
  Engine::End();
}

}

std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::make_unique<wanderer::Wanderer>(); }