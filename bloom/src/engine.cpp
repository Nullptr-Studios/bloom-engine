#include "engine.hpp"
#include "simple_render_system.hpp"
#include "render/buffer.hpp"

namespace bloom {

struct GlobalUBO {
  alignas(16) glm::mat4 projectionMatrix = glm::mat4(1.0f);
  alignas(16) glm::vec4 colorUniform = {1.0f, 0.0f, 1.0f, 1.0f};
};

Engine::Engine() { }

// region Begin
void Engine::Init() {
  glfwInit();
  Log::Init();

  m_window = new Window(800, 800, "Bloom");
  m_window->SetEventCallback([this]<typename T0>(T0 && ph1) { OnEvent(std::forward<T0>(ph1)); });
  m_window->OnBegin();

  factory = std::make_unique<Factory>();

  m_devices = std::make_unique<render::Devices>(*m_window);
  factory->SetDevice(m_devices.get());

  // TODO: I probably want to move all this elsewhere
  m_globalPool = render::DescriptorPool::Builder(*m_devices)
    .SetMaxSets(render::SwapChain::MAX_FRAMES_IN_FLIGHT)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, render::SwapChain::MAX_FRAMES_IN_FLIGHT)
  .Build();

  m_UBOBuffers.resize(render::SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < m_UBOBuffers.size(); i++) {
    m_UBOBuffers[i] = new render::Buffer(
      *m_devices,
      sizeof(GlobalUBO),
      1, // this will change later on -x
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
    );
    m_UBOBuffers[i]->Map();
  }

  // GLOBAL DESCRIPTOR SET ------------------------------------------
  auto globalSetLayout = render::DescriptorSetLayout::Builder(*m_devices)
    .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
  .Build();

  // MATERIAL DESCRIPTOR SET ----------------------------------------
  m_materialSetLayout = render::DescriptorSetLayout::Builder(*m_devices)
    .AddBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // albedo
    .AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // RMO
    .AddBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // normal
    .AddBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // emission
  .Build();

  m_globalDescriptorSets.resize(render::SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < m_globalDescriptorSets.size(); i++) {
    auto bufferInfo = m_UBOBuffers[i]->DescriptorInfo();
    render::DescriptorWriter(*globalSetLayout, *m_globalPool)
      .WriteBuffer(0, &bufferInfo)
    .Build(m_globalDescriptorSets[i]);
  }

  m_renderer = std::make_unique<render::Renderer>(m_window, m_devices.get());
  m_simpleRenderSystem = new SimpleRenderSystem(m_devices.get());
  m_simpleRenderSystem->Begin(m_renderer->GetRenderPass(), globalSetLayout->GetDescriptorSetLayout(), m_materialSetLayout->GetDescriptorSetLayout());

  OnBegin();

  for (auto& object : factory->GetObjects())
    object.second->OnBegin();
}

// region Tick
void Engine::OnTick() {
  m_deltaTime = static_cast<float>(m_window->GetDeltaTime());
  m_window->OnTick();

  // Objects game loop
  for (auto& object : factory->GetObjects())
    object.second->OnTick(m_deltaTime);
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
      m_globalDescriptorSets[frameIndex]
    };

    // update
    GlobalUBO UBO{};
    UBO.projectionMatrix = m_activeCamera->GetProjection() * m_activeCamera->GetView();
    //UBO.colorUniform = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    m_UBOBuffers[frameIndex]->WriteToIndex(&UBO, 0);
    m_UBOBuffers[frameIndex]->Flush();

    // render
    m_renderer->BeginRenderPass(commandBuffer);
    m_simpleRenderSystem->RenderObjects(frameInfo, factory->GetRenderables());
    m_renderer->EndRenderPass(commandBuffer);
    m_renderer->EndFrame();
  }
}

// region End
void Engine::OnClose() {
  vkDeviceWaitIdle(m_devices->Device());
  for (auto& buffer : m_UBOBuffers)
    // I got an error with the std::unique_ptr, so I changed it to a raw pointer -x
    delete buffer;
  delete m_window;
}

// region Events
void Engine::OnEvent(const Event &e) const {
  // BLOOM_INFO("{0}", e.ToString());

  if (e.GetEventType() == EventType::WindowClose)
    m_window->OnCloseWindow();
}

// TODO: Avoid having to create this shit
void Engine::OnBegin() { }

}
