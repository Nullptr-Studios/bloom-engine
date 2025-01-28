#include "ui_layer.hpp"
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>
#include "src/render/renderer.hpp"
#include "src/events/game_event.hpp"
#include "src/events/key_event.hpp"
#include "src/events/mouse_event.hpp"
#include "src/render/descriptors.hpp"

#include "src/factory.hpp"

namespace bloom::ui {

void UILayer::OnAttach() {
  m_device = render::Devices::GetInstance();

  // TODO: This is completely overkill
  m_imguiPool = render::DescriptorPool::Builder(*m_device)
    .SetMaxSets(1000)
    .SetPoolFlags(VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT)
    .AddPoolSize(VK_DESCRIPTOR_TYPE_SAMPLER, 1000)
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
    ImGui::Begin((std::to_string(id) + ": " + obj->GetName()).c_str());
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

// region Events
void UILayer::OnEvent(Event &e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<MouseButtonPressedEvent>(EVENT_BIND(UILayer::OnMouseButtonPressed));
  dispatcher.Dispatch<MouseButtonReleasedEvent>(EVENT_BIND(UILayer::OnMouseButtonReleased));
  dispatcher.Dispatch<MouseMovedEvent>(EVENT_BIND(UILayer::OnMouseMoved));
  dispatcher.Dispatch<MouseScrolledEvent>(EVENT_BIND(UILayer::OnMouseScrolled));
  dispatcher.Dispatch<KeyPressedEvent>(EVENT_BIND(UILayer::OnKeyPressed));
  dispatcher.Dispatch<KeyTypedEvent>(EVENT_BIND(UILayer::OnKeyTyped));
  dispatcher.Dispatch<KeyReleasedEvent>(EVENT_BIND(UILayer::OnKeyReleased));
  dispatcher.Dispatch<WindowResizeEvent>(EVENT_BIND(UILayer::OnWindowResized));
}

// Right now, all functions return false, we should work on this so events don't propagate down -x
// TODO: Make sure events don't propagate down
bool UILayer::OnMouseButtonPressed(const MouseButtonPressedEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton()] = true;

  return false;
}

bool UILayer::OnMouseButtonReleased(const MouseButtonReleasedEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseDown[e.GetMouseButton()] = false;

  return false;
}

bool UILayer::OnMouseMoved(const MouseMovedEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MousePos = ImVec2(e.GetX(), e.GetY());

  return false;
}

bool UILayer::OnMouseScrolled(const MouseScrolledEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  io.MouseWheel = e.GetYOffset();
  io.MouseWheelH = e.GetXOffset();

  return false;
}

bool UILayer::OnKeyPressed(const KeyPressedEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  auto keyEvent = TranslateToImGui(e.GetKeyCode());
  io.AddKeyEvent(keyEvent, true);

  // MODIFIERS
  // Windows key probably isn't needed??? -x
  io.KeyCtrl = keyEvent == ImGuiKey_LeftCtrl || keyEvent == ImGuiKey_RightCtrl;
  io.KeyShift = keyEvent == ImGuiKey_LeftShift || keyEvent == ImGuiKey_RightShift;
  io.KeyAlt = keyEvent == ImGuiKey_LeftAlt || keyEvent == ImGuiKey_RightAlt;
  io.KeySuper = keyEvent == ImGuiKey_LeftSuper || keyEvent == ImGuiKey_RightSuper;

  return false;
}

bool UILayer::OnKeyReleased(const KeyReleasedEvent &e) {
  ImGuiIO& io = ImGui::GetIO();
  auto keyEvent = TranslateToImGui(e.GetKeyCode());
  io.AddKeyEvent(TranslateToImGui(keyEvent), false);

  // MODIFIERS
  io.KeyCtrl = (keyEvent == ImGuiKey_LeftCtrl || keyEvent == ImGuiKey_RightCtrl) ? false : io.KeyCtrl;
  io.KeyShift = (keyEvent == ImGuiKey_LeftShift || keyEvent == ImGuiKey_RightShift) ? false : io.KeyShift;
  io.KeyAlt = (keyEvent == ImGuiKey_LeftAlt || keyEvent == ImGuiKey_RightAlt) ? false : io.KeyAlt;
  io.KeySuper = (keyEvent == ImGuiKey_LeftSuper || keyEvent == ImGuiKey_RightSuper) ? false : io.KeySuper;

  return false;
}

bool UILayer::OnKeyTyped(const KeyTypedEvent & e) {
  ImGuiIO& io = ImGui::GetIO();
  int keycode = e.GetKeyCode();
  if (keycode > 0 && keycode < 0x10000)
    io.AddInputCharacter(static_cast<unsigned short>(keycode));

  return false;
}

bool UILayer::OnWindowResized(const WindowResizeEvent &e) {
  ImGuiIO &io = ImGui::GetIO();
  io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
  io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

  return false;
}

ImGuiKey UILayer::TranslateToImGui(int keycode) {
  switch (keycode) {
    case GLFW_KEY_TAB: return ImGuiKey_Tab;
    case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
    case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
    case GLFW_KEY_UP: return ImGuiKey_UpArrow;
    case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
    case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
    case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
    case GLFW_KEY_HOME: return ImGuiKey_Home;
    case GLFW_KEY_END: return ImGuiKey_End;
    case GLFW_KEY_INSERT: return ImGuiKey_Insert;
    case GLFW_KEY_DELETE: return ImGuiKey_Delete;
    case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
    case GLFW_KEY_SPACE: return ImGuiKey_Space;
    case GLFW_KEY_ENTER: return ImGuiKey_Enter;
    case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
    case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
    case GLFW_KEY_COMMA: return ImGuiKey_Comma;
    case GLFW_KEY_MINUS: return ImGuiKey_Minus;
    case GLFW_KEY_PERIOD: return ImGuiKey_Period;
    case GLFW_KEY_SLASH: return ImGuiKey_Slash;
    case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
    case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
    case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
    case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
    case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
    case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
    case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
    case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
    case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
    case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
    case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
    case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
    case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
    case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
    case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
    case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
    case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
    case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
    case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
    case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
    case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
    case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
    case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
    case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
    case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
    case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
    case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
    case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
    case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
    case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
    case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
    case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
    case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
    case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
    case GLFW_KEY_MENU: return ImGuiKey_Menu;
    case GLFW_KEY_0: return ImGuiKey_0;
    case GLFW_KEY_1: return ImGuiKey_1;
    case GLFW_KEY_2: return ImGuiKey_2;
    case GLFW_KEY_3: return ImGuiKey_3;
    case GLFW_KEY_4: return ImGuiKey_4;
    case GLFW_KEY_5: return ImGuiKey_5;
    case GLFW_KEY_6: return ImGuiKey_6;
    case GLFW_KEY_7: return ImGuiKey_7;
    case GLFW_KEY_8: return ImGuiKey_8;
    case GLFW_KEY_9: return ImGuiKey_9;
    case GLFW_KEY_A: return ImGuiKey_A;
    case GLFW_KEY_B: return ImGuiKey_B;
    case GLFW_KEY_C: return ImGuiKey_C;
    case GLFW_KEY_D: return ImGuiKey_D;
    case GLFW_KEY_E: return ImGuiKey_E;
    case GLFW_KEY_F: return ImGuiKey_F;
    case GLFW_KEY_G: return ImGuiKey_G;
    case GLFW_KEY_H: return ImGuiKey_H;
    case GLFW_KEY_I: return ImGuiKey_I;
    case GLFW_KEY_J: return ImGuiKey_J;
    case GLFW_KEY_K: return ImGuiKey_K;
    case GLFW_KEY_L: return ImGuiKey_L;
    case GLFW_KEY_M: return ImGuiKey_M;
    case GLFW_KEY_N: return ImGuiKey_N;
    case GLFW_KEY_O: return ImGuiKey_O;
    case GLFW_KEY_P: return ImGuiKey_P;
    case GLFW_KEY_Q: return ImGuiKey_Q;
    case GLFW_KEY_R: return ImGuiKey_R;
    case GLFW_KEY_S: return ImGuiKey_S;
    case GLFW_KEY_T: return ImGuiKey_T;
    case GLFW_KEY_U: return ImGuiKey_U;
    case GLFW_KEY_V: return ImGuiKey_V;
    case GLFW_KEY_W: return ImGuiKey_W;
    case GLFW_KEY_X: return ImGuiKey_X;
    case GLFW_KEY_Y: return ImGuiKey_Y;
    case GLFW_KEY_Z: return ImGuiKey_Z;
    case GLFW_KEY_F1: return ImGuiKey_F1;
    case GLFW_KEY_F2: return ImGuiKey_F2;
    case GLFW_KEY_F3: return ImGuiKey_F3;
    case GLFW_KEY_F4: return ImGuiKey_F4;
    case GLFW_KEY_F5: return ImGuiKey_F5;
    case GLFW_KEY_F6: return ImGuiKey_F6;
    case GLFW_KEY_F7: return ImGuiKey_F7;
    case GLFW_KEY_F8: return ImGuiKey_F8;
    case GLFW_KEY_F9: return ImGuiKey_F9;
    case GLFW_KEY_F10: return ImGuiKey_F10;
    case GLFW_KEY_F11: return ImGuiKey_F11;
    case GLFW_KEY_F12: return ImGuiKey_F12;
    case GLFW_KEY_F13: return ImGuiKey_F13;
    case GLFW_KEY_F14: return ImGuiKey_F14;
    case GLFW_KEY_F15: return ImGuiKey_F15;
    case GLFW_KEY_F16: return ImGuiKey_F16;
    case GLFW_KEY_F17: return ImGuiKey_F17;
    case GLFW_KEY_F18: return ImGuiKey_F18;
    case GLFW_KEY_F19: return ImGuiKey_F19;
    case GLFW_KEY_F20: return ImGuiKey_F20;
    case GLFW_KEY_F21: return ImGuiKey_F21;
    case GLFW_KEY_F22: return ImGuiKey_F22;
    case GLFW_KEY_F23: return ImGuiKey_F23;
    case GLFW_KEY_F24: return ImGuiKey_F24;
    default: return ImGuiKey_None;
  }
}

}
