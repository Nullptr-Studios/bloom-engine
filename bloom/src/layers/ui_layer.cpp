#include "ui_layer.hpp"
#include "src/render/renderer.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

namespace bloom::ui {

void UILayer::OnAttach() {
  m_device = render::Devices::GetInstance();

  m_imguiPool = render::DescriptorPool::Builder(*m_device)
    .SetMaxSets(1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLER, 1)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000)
  .Build();

  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui_ImplGlfw_InitForVulkan(Window::GetInstance()->GetWindow(), true);
  ImGui_ImplVulkan_InitInfo initInfo = {};
  initInfo.Instance = m_device->GetVkInstance();
  initInfo.PhysicalDevice = m_device->PhysicalDevice();
  initInfo.Device = m_device->Device();
  initInfo.QueueFamily = m_device->FindPhysicalQueueFamilies().graphicsFamily;
  initInfo.Queue = m_device->GraphicsQueue();
  initInfo.PipelineCache = VK_NULL_HANDLE;
  initInfo.DescriptorPool = m_imguiPool->GetVkDescriptorPool();
  initInfo.RenderPass = render::Renderer::GetInstance()->GetRenderPass();
  initInfo.Subpass = 0;
  initInfo.MinImageCount = 2;
  initInfo.ImageCount = 2;
  initInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
  initInfo.Allocator = nullptr;
  initInfo.CheckVkResultFn = nullptr;
  ImGui_ImplVulkan_Init(&initInfo);
}

void UILayer::OnBegin() {

}

void UILayer::OnTick(float deltaTime) {
  ImGui_ImplVulkan_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::ShowDemoWindow();
}

void UILayer::OnRender(render::FrameInfo frameInfo) {
  ImGui::Render();
  ImDrawData* draw_data = ImGui::GetDrawData();
  ImGui_ImplVulkan_RenderDrawData(draw_data, frameInfo.commandBuffer);
}

void UILayer::OnDetach() {
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void UILayer::OnEvent(const Event &e) {

}

}
