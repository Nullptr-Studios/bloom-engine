#include "engine.hpp"
#include "render/model.hpp"

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
  m_simpleRenderSystem = new SimpleRenderSystem(m_devices.get());
  m_simpleRenderSystem->Begin(m_renderer->GetRenderPass());

  OnBegin();

  for (auto& object : factory->GetObjects()) {
    object.second->Begin();
  }
}

void Engine::Tick() {
  m_deltaTime = m_window->GetDeltaTime();
  m_window->OnTick();

  // Objects game loop
  for (auto& object : factory->GetObjects()) {
    object.second->Tick(m_deltaTime);
  }
}

void Engine::Render() {
  if (auto commandBuffer = m_renderer->BeginFrame()) {
    m_renderer->BeginRenderPass(commandBuffer);
    m_simpleRenderSystem->RenderObjects(commandBuffer, factory->GetRenderables(), m_activeCamera.get());
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
  // BLOOM_INFO("{0}", e.ToString());

  if (e.GetEventType() == EventType::WindowClose) {
    m_window->CloseWindow();
  }
}

void Engine::OnBegin() { }

}
