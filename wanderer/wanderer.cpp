#include "wanderer.hpp"
#include "editor_camera.hpp"

using namespace bloom;
namespace wanderer {

std::unique_ptr<bloom::render::Model> createCubeModel(bloom::render::Devices* device, glm::vec3 offset) {
  render::Model::Builder builder;
  builder.vertices = {
    // left face (white)
    {{-.5f, -.5f, -.5f}, {0.0f, 0.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},
    {{-.5f, .5f, .5f},   {1.0f, 1.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},
    {{-.5f, -.5f, .5f},  {0.0f, 1.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},
    {{-.5f, .5f, -.5f},  {1.0f, 0.0f}, {0.9f, 0.9f, 0.9f, 1.0f}},
 
    // right face (yellow)
    {{.5f, -.5f, -.5f},  {0.0f, 0.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
    {{.5f, .5f, .5f},    {1.0f, 1.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
    {{.5f, -.5f, .5f},   {0.0f, 1.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
    {{.5f, .5f, -.5f},   {1.0f, 0.0f}, {0.8f, 0.8f, 0.1f, 1.0f}},
 
    // top face (orange)
    {{-.5f, -.5f, -.5f}, {0.0f, 0.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
    {{.5f, -.5f, .5f},   {1.0f, 1.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
    {{-.5f, -.5f, .5f},  {0.0f, 1.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
    {{.5f, -.5f, -.5f},  {1.0f, 0.0f}, {0.9f, 0.6f, 0.1f, 1.0f}},
 
    // bottom face (red)
    {{-.5f, .5f, -.5f},  {0.0f, 0.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
    {{.5f, .5f, .5f},    {1.0f, 1.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
    {{-.5f, .5f, .5f},   {0.0f, 1.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
    {{.5f, .5f, -.5f},   {1.0f, 0.0f}, {0.8f, 0.1f, 0.1f, 1.0f}},
 
    // nose face (blue)
    {{-.5f, -.5f, 0.5f}, {0.0f, 0.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},
    {{.5f, .5f, 0.5f},   {1.0f, 1.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},
    {{-.5f, .5f, 0.5f},  {0.0f, 1.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},
    {{.5f, -.5f, 0.5f},  {1.0f, 0.0f}, {0.1f, 0.1f, 0.8f, 1.0f}},
 
    // tail face (green)
    {{-.5f, -.5f, -0.5f}, {0.0f, 0.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},
    {{.5f, .5f, -0.5f},   {1.0f, 1.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},
    {{-.5f, .5f, -0.5f},  {0.0f, 1.0f}, {0.1f, 0.8f, 0.1f, 1.0f}},
    {{.5f, -.5f, -0.5f},  {1.0f, 0.0f}, {0.1f, 0.8f, 0.1f, 1.0f}}

  };
  for (auto& v : builder.vertices) {
    v.position += offset;
  }

  builder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                     12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

  return std::make_unique<bloom::render::Model>(device, builder);
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