#include "ui_layer.hpp"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "src/render/renderer.hpp"

#include "src/factory.hpp"

namespace bloom::ui {

void UILayer::OnAttach() {
  m_device = render::Devices::GetInstance();

  m_imguiPool = render::DescriptorPool::Builder(*m_device)
    .SetMaxSets(1000)
    .SetPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
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
  ImGui_ImplGlfw_InitForVulkan(Window::GetInstance()->GetWindow(), false);
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

  // TODO: This is a hack
  for (auto &[id, obj] : Factory::GetInstance()->GetObjects()) {
    ImGui::Begin(("Object " + std::to_string(id) + ": " + obj->GetName()).c_str());
    obj->PropertiesPanel();
    ImGui::End();
  }
}

void UILayer::OnRender(render::FrameInfo frameInfo) {
  ImGui::Render();
  ImDrawData* drawData = ImGui::GetDrawData();
  // TODO: Maybe we should make our own render system for UI
  ImGui_ImplVulkan_RenderDrawData(drawData, frameInfo.commandBuffer);
}

void UILayer::OnDetach() {
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void UILayer::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<MouseButtonPressedEvent>(EVENT_BIND(UILayer::OnMouseButtonPressed));
  dispatcher.Dispatch<MouseButtonReleasedEvent>(EVENT_BIND(UILayer::OnMouseButtonReleased));
  dispatcher.Dispatch<MouseMovedEvent>(EVENT_BIND(UILayer::OnMouseMoved));
  dispatcher.Dispatch<MouseScrolledEvent>(EVENT_BIND(UILayer::OnMouseScrolled));
  dispatcher.Dispatch<KeyPressedEvent>(EVENT_BIND(UILayer::OnKeyPressed));
  dispatcher.Dispatch<KeyReleasedEvent>(EVENT_BIND(UILayer::OnKeyReleased));
  dispatcher.Dispatch<WindowResizeEvent>(EVENT_BIND(UILayer::OnWindowResized));
}

// Right now, all functions return false, we should work on this so events don't propagate down -x
bool UILayer::OnMouseButtonPressed(MouseButtonPressedEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton()] = true;

  return false;
}

bool UILayer::OnMouseButtonReleased(MouseButtonReleasedEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton()] = false;

  return false;
}

bool UILayer::OnMouseMoved(MouseMovedEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2(e.GetX(), e.GetY());

  return false;
}

bool UILayer::OnMouseScrolled(MouseScrolledEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheel = e.GetYOffset();
  io.MouseWheelH = e.GetXOffset();

  return false;
}

bool UILayer::OnKeyPressed(KeyPressedEvent &e) {


  return false;
}

bool UILayer::OnKeyReleased(KeyReleasedEvent &e) {


  return false;
}

bool UILayer::OnWindowResized(WindowResizeEvent &e) {


  return false;
}

}
