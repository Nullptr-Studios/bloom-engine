#include "game_layer.hpp"

#include "src/engine.hpp"
#include "src/render/descriptor_layouts.hpp"
#include "src/render/renderer.hpp"
#include "src/render/swap_chain.hpp"
#include "src/render/systems/simple_render_system.hpp"

namespace bloom::render {

// region UBO
struct GlobalUBO {
  alignas(16) glm::mat4 projectionMatrix = glm::mat4(1.0f);
  alignas(16) glm::vec4 colorUniform = {1.0f, 0.0f, 1.0f, 1.0f};
};

// region OnAttach
void GameLayer::OnAttach() {
  m_factory = std::make_unique<Factory>();

  // TODO: I probably want to move all this elsewhere
  m_globalPool = DescriptorPool::Builder(*Devices::GetInstance())
   .SetMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
   .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
  .Build();

  m_UBOBuffers.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < (int)m_UBOBuffers.size(); i++) {
    m_UBOBuffers[i] = new Buffer(*Devices::GetInstance(), sizeof(GlobalUBO),
      1, // this will change later on -x
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    m_UBOBuffers[i]->Map();
  }

  // region Layouts
  // GLOBAL DESCRIPTOR SET LAYOUT ------------------------------------------
  m_globalSetLayout = DescriptorSetLayout::Builder(*Devices::GetInstance())
    .AddBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
  .Build();
  Engine::SetUBOLayout(m_globalSetLayout.get());

  m_globalDescriptorSets.resize(SwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < (int)m_globalDescriptorSets.size(); i++) {
    auto bufferInfo = m_UBOBuffers[i]->DescriptorInfo();
    DescriptorWriter(*m_globalSetLayout, *m_globalPool)
      .WriteBuffer(0, &bufferInfo)
    .Build(m_globalDescriptorSets[i]);
  }

  // MATERIAL DESCRIPTOR SET LAYOUT ----------------------------------------
  m_materialSetLayout =
      DescriptorSetLayout::Builder(*Devices::GetInstance())
        .AddBinding(0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // albedo
        .AddBinding(1, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // RMO
        .AddBinding(2, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // normal
        .AddBinding(3, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT) // emission
      .Build();
  Engine::SetMaterialLayout(m_materialSetLayout.get());
  
  m_simpleRenderSystem = new SimpleRenderSystem(Devices::GetInstance());
  m_simpleRenderSystem->Begin(Renderer::GetInstance()->GetRenderPass(), m_globalSetLayout->GetDescriptorSetLayout(),
                              m_materialSetLayout->GetDescriptorSetLayout());
}

// region OnBegin
void GameLayer::OnBegin() {
  // Objects begin
  for (auto& object : m_factory->GetObjects())
    object.second->OnBegin();
}

// region OnTick
void GameLayer::OnTick(float deltaTime) {
  // Objects game loop
  for (auto &[fst, snd] : m_factory->GetObjects())
    snd->OnTick(deltaTime);
}

// region OnRender
void GameLayer::OnRender(FrameInfo frameInfo) {
  frameInfo.globalDescriptorSet = m_globalDescriptorSets[frameInfo.frameIndex];

  // update
  GlobalUBO UBO{};
  UBO.projectionMatrix = frameInfo.activeCamera->GetProjection() * frameInfo.activeCamera->GetView();
  m_UBOBuffers[frameInfo.frameIndex]->WriteToIndex(&UBO, 0);
  (void)m_UBOBuffers[frameInfo.frameIndex]->Flush();

  // render
  m_simpleRenderSystem->RenderObjects(frameInfo, m_factory->GetRenderables());
}

// region OnDetach
void GameLayer::OnDetach() {
  for (auto& buffer : m_UBOBuffers)
    // I got an error with the std::unique_ptr, so I changed it to a raw pointer -x
      delete buffer;
}

// region OnEvent
void GameLayer::OnEvent(Event& e) {
  for (auto& [fst, snd] : m_factory->GetObjects())
    snd->OnEvent(e);
}

}
