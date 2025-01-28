#include "engine.hpp"
#include "render/descriptor_layouts.hpp"
#include "layers/layer_stack.hpp"
#include "render/devices.hpp"
#include "render/renderer.hpp"
#include "window.hpp"
#include "events/game_event.hpp"

#include "layers/game_layer.hpp"
#include "layers/ui_layer.hpp"
#include "render/frame_info.hpp"
#include "render/systems/simple_render_system.hpp"

namespace bloom {

render::DescriptorLayouts Engine::m_descriptorLayouts;

Engine::Engine() { }

// region Begin
void Engine::Init() {
  Log::Init();

  m_window = new Window(800, 800, "Bloom");
  m_window->SetEventCallback(EVENT_BIND(Engine::OnEvent));
  m_window->OnBegin();

  m_devices = std::make_unique<render::Devices>(*m_window);
  m_renderer = std::make_unique<render::Renderer>(m_window, m_devices.get());
  PushLayer(new render::GameLayer("Game Layer"));
  PushLayer(new ui::UILayer("UI Layer"));

  OnBegin();

  int layerIndex = 0;
  for (auto &layer : m_layerStack) {
    layer->OnBegin();
    BLOOM_LOG("{0}: {1}", layerIndex, layer->GetName());
    layerIndex++;
  }
}

// region Tick
void Engine::OnTick() {
  m_deltaTime = static_cast<float>(m_window->GetDeltaTime());
  m_window->OnTick();

  for (auto &layer : m_layerStack)
    layer->OnTick(m_deltaTime);
}

// region Render
void Engine::OnRender() {
  if (auto commandBuffer = m_renderer->BeginFrame()) {
    char frameIndex = static_cast<char>(m_renderer->GetFrameIndex());
    render::FrameInfo frameInfo{
      frameIndex,
      m_deltaTime,
      commandBuffer,
      // TODO: better camera management than this
      // Look for how cinemachine handles the camera used for the brain -x
      m_activeCamera.get(),
    };

    // render
    m_renderer->BeginRenderPass(commandBuffer);

    for (auto &layer : m_layerStack)
      layer->OnRender(frameInfo);

    m_renderer->EndRenderPass(commandBuffer);
    m_renderer->EndFrame();
  }
}

// region End
void Engine::OnClose() {
  vkDeviceWaitIdle(m_devices->Device());
  for (auto &layer : m_layerStack)
    m_layerStack.PopLayer(layer);
  delete m_window;
}

// region Events
void Engine::OnEvent(Event &e) {
  if (e.GetEventType() == EventType::KeyTyped)
    BLOOM_INFO("{0}", e.ToString());

  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(EVENT_BIND(Engine::ExitEngine));
  if (e.IsHandled()) return;

  for (auto i = m_layerStack.end(); i != m_layerStack.begin();) {
    (*--i)->OnEvent(e);
    if (e.IsHandled())
      break;
  }
}

bool Engine::ExitEngine(Event& e) {
  m_shouldClose = true;
  return true;
}

// region Layer Management
void Engine::PushLayer(Layer *layer) {
  m_layerStack.PushLayer(layer);
}
void Engine::PushOverlay(Layer *overlay) {
  m_layerStack.PushOverlay(overlay);
}

} // namespace bloom
