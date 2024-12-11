#include "engine.hpp"
#include "render/model.hpp"
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
}

void Engine::Tick() {
  m_deltaTime = m_window->GetDeltaTime();
  m_window->OnTick();
  BLOOM_LOG("{0}", 1/m_deltaTime);
}

void Engine::Render() {
  if (auto commandBuffer = m_renderer->BeginFrame()) {
    m_renderer->BeginRenderPass(commandBuffer);
    m_simpleRenderSystem->RenderObjects(commandBuffer, gameObjects);
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

void Engine::LoadObjects() {
  std::vector<render::Model::Vertex> vertices {
  {{ 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f, 1.0f}},
  {{ 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f, 1.0f}},
  {{-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f}}
};

  auto m_model = std::make_shared<render::Model>(m_devices.get(), vertices);

  auto triangle = factory->CreateObject<Object>();
  triangle.model = m_model;
  triangle.color = {0.8f, 0.1f, 0.8f};
  triangle.transform.position.x = 0.0f;
  triangle.transform.scale = {1.0f, 1.0f};
  triangle.transform.rotation = 0.25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

} // namespace bloom
