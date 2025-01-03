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

  std::shared_ptr<render::Model> model = render::Model::CreateModel("resources/models/teapot_smooth.obj");

  auto cube = BLOOM_FACTORY->CreateObject<Actor>("Teapot 1");
  cube->model = model;
  cube->transform.position = {0.0f, 0.0f, -4.0f};
  cube->transform.scale = {1.0f, 1.0f, 1.0f};
  // TODO: This hack should be done in a better way, probably on the actor class
  cube->LoadTextures(GetDescriptorLayouts().materialLayout, "resources/textures/cat.png");
}

void Wanderer::OnTick() {
  Engine::OnTick();
}

void Wanderer::OnClose() {
  Engine::OnClose();
}

}

std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::make_unique<wanderer::Wanderer>(); }