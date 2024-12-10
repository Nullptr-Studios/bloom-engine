#include "engine.hpp"

#include "glm/gtc/constants.hpp"

namespace bloom {

Engine::Engine() { }
Engine::~Engine() {
  vkDestroyPipelineLayout(m_devices->device(), m_pipelineLayout, nullptr);
}

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
  CreatePipelineLayout();
  CreatePipeline();
}

void Engine::Tick() {
  m_window->OnTick();
}

void Engine::Render() {
  if (auto commandBuffer = m_renderer->BeginFrame()) {
    m_renderer->BeginRenderPass(commandBuffer);
    RenderObjects(commandBuffer);
    m_renderer->EndRenderPass(commandBuffer);
    m_renderer->EndFrame();
  }
}

void Engine::End() {
  vkDeviceWaitIdle(m_devices->device());
  delete m_window;
}

void Engine::OnEvent(Event &e) {
  BLOOM_INFO("{0}", e.ToString());

  if (e.GetEventType() == EventType::WindowClose) {
    m_window->CloseWindow();
  }
}

void Engine::CreatePipelineLayout() {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  auto result = vkCreatePipelineLayout(m_devices->device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout);

  if (result != VK_SUCCESS) {
    BLOOM_CRITICAL("Failed to create pipeline layout");
  }
}

void Engine::CreatePipeline() {
  if (m_pipelineLayout == nullptr) BLOOM_CRITICAL("Pipeline layout is null");
  render::PipelineConfiguration pipelineConfig{};
  render::Pipeline::defaultPipelineConfig(pipelineConfig);
  // Tells what layout to expect to the render buffer
  pipelineConfig.renderPass = m_renderer->GetRenderPass();
  pipelineConfig.pipelineLayout = m_pipelineLayout;
  m_pipeline = std::make_unique<render::Pipeline>(*m_devices, "resources/shaders/default.vert.spv", "resources/shaders/default.frag.spv", pipelineConfig);
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

void Engine::RenderObjects(VkCommandBuffer commandBuffer) {
  m_pipeline->Bind(commandBuffer);

  for (auto& obj : gameObjects) {
    obj.transform.rotation = glm::mod(obj.transform.rotation + 0.0001f, glm::two_pi<float>());

    SimplePushConstantData push{};
    push.offset = obj.transform.position;
    push.color = obj.color;
    push.transform = obj.transform.mat2();

    vkCmdPushConstants(
      commandBuffer,
      m_pipelineLayout,
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
      0,
      sizeof(SimplePushConstantData),
      &push
    );

    obj.model->Bind(commandBuffer);
    obj.model->Draw(commandBuffer);
  }
}

} // namespace bloom
