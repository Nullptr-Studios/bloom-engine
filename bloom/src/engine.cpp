#include "engine.hpp"
#include "render/model.hpp"
#include "render/texture.hpp"
#include "glm/gtc/constants.hpp"

namespace bloom {

#pragma region Game Loop

void Engine::Begin() {
  glfwInit();
	Log::Init();

  m_window = new Window(800, 800, "Bloom");
  m_window->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
  m_window->OnInit();

  factory = std::make_unique<Factory>();

  m_devices = std::make_unique<render::Devices>(*m_window);
  m_renderer = std::make_unique<render::Renderer>(m_window, m_devices.get());
  LoadObjects();
  m_simpleRenderSystem = new SimpleRenderSystem(m_devices.get());
  m_simpleRenderSystem->Begin(m_renderer->GetRenderPass());

  m_camera = Camera();
  m_camera.SetViewDirection(glm::vec3(0.0f), glm::vec3(-0.5f, 0.0f, -1.0f));
}

void Engine::Tick() {
  float aspect = m_renderer->GetAspectRatio();
  m_camera.SetPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 100.0f);

  m_deltaTime = m_window->GetDeltaTime();
  m_window->OnTick();
  BLOOM_LOG("{0}FPS", 1/m_deltaTime);

  m_rotation += m_deltaTime * 0.1f;
  if (m_rotation > .7f) m_rotation = -.7f;
  m_camera.SetViewDirection(glm::vec3(0.0f), glm::vec3(m_rotation, 0.0f, -1.0f));
}

void Engine::Render() {
  if (auto commandBuffer = m_renderer->BeginFrame()) {
    m_renderer->BeginRenderPass(commandBuffer);
    m_simpleRenderSystem->RenderObjects(commandBuffer, gameObjects, m_camera);
    m_renderer->EndRenderPass(commandBuffer);
    m_renderer->EndFrame();
  }
}

void Engine::End() const {
  vkDeviceWaitIdle(m_devices->device());
  delete m_window;
}

#pragma endregion // -------------------------------------------------------------------------------------------------

void Engine::OnEvent(const Event &e) const {
  BLOOM_INFO("{0}", e.ToString());

  if (e.GetEventType() == EventType::WindowClose) {
    m_window->CloseWindow();
  }
}

std::unique_ptr<render::Model> createCubeModel(render::Devices* device, glm::vec3 offset) {
  std::vector<render::Model::Vertex> vertices = {
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
 
      // top face (orange, remember y axis points down)
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
  return std::make_unique<render::Model>(device, vertices);
}

void Engine::LoadObjects() {
  std::shared_ptr<render::Model> model = createCubeModel(m_devices.get(), {0.0f, 0.0f, 0.0f});

  auto cube = factory->CreateObject<Object>();
  cube.model = model;
  cube.transform.position = {0.0f, 0.0f, -2.5f};
  cube.transform.scale = {0.5f, 0.5f, 0.5f};
  cube.texture = new render::Texture(m_devices.get(), "resources/textures/cat.png");
  gameObjects.push_back(std::move(cube));
}

} // namespace bloom
