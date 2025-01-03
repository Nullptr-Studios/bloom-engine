#include "wanderer.hpp"
#include "editor_camera.hpp"
#include "src/factory.hpp"
#include "src/layers/game_layer.hpp"
#include "src/render/model.hpp"

using namespace bloom;
namespace wanderer {

// TODO: This needs a different name
void Wanderer::OnBegin() {
  m_camera = BLOOM_FACTORY->CreateObject<EditorCamera>("Camera");
  m_activeCamera = m_camera;

  auto teapot = BLOOM_FACTORY->CreateObject<Actor>("Teapot");
  teapot->model = render::Model::LoadObj("resources/models/teapot_smooth.obj");;
  teapot->transform.position = {1.0f, 0.0f, -4.0f};
  teapot->transform.scale = {1.0f, -1.0f, 1.0f};
  teapot->LoadTextures(GetDescriptorLayouts().materialLayout, "resources/textures/cat.png");

  auto hardteapot = BLOOM_FACTORY->CreateObject<Actor>("HardTeapot");
  hardteapot->model = render::Model::LoadObj("resources/models/teapot.obj");
  hardteapot->transform.position = {-1.0f, 0.0f, -4.0f};
  hardteapot->transform.scale = {1.0f, -1.0f, 1.0f};
  hardteapot->LoadTextures(GetDescriptorLayouts().materialLayout, "resources/textures/bloom.png");
}

void Wanderer::OnTick() {
  Engine::OnTick();
}

void Wanderer::OnClose() {
  Engine::OnClose();
}

}

std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::make_unique<wanderer::Wanderer>(); }