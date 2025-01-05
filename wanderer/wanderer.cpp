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
  m_camera->transform.position = {0.0f, -0.8f, 2.0f};
  m_camera->transform.rotation = {glm::radians(-17.0f), 0.0f, 0.0f};
  m_activeCamera = m_camera;

  auto teapot = BLOOM_FACTORY->CreateObject<Actor>("Teapot");
  teapot->model = render::Model::LoadOBJ("resources/models/teapot_smooth.obj");;
  teapot->transform.position = {.50f, 0.0f, 0.0f};
  teapot->transform.scale = glm::vec3(0.5f, -0.5f, 0.5f);
  teapot->LoadTextures(GetDescriptorLayouts().materialLayout, "resources/textures/cat.png");

  auto hardteapot = BLOOM_FACTORY->CreateObject<Actor>("HardTeapot");
  hardteapot->model = render::Model::LoadOBJ("resources/models/teapot.obj");
  hardteapot->transform.position = {-.50f, 0.0f, 0.0f};
  hardteapot->transform.scale = glm::vec3(0.5f, -0.5f, 0.5f);
  hardteapot->LoadTextures(GetDescriptorLayouts().materialLayout, "resources/textures/bloom.png");

  auto floor = BLOOM_FACTORY->CreateObject<Actor>("Floor");
  floor->model = render::Model::LoadOBJ("resources/models/plane.obj");
  floor->transform.position = {0.0f, 0.0f, 0.0f};
  floor->transform.scale = glm::vec3(3.0f);
  floor->LoadTextures(GetDescriptorLayouts().materialLayout);
}

void Wanderer::OnTick() {
  Engine::OnTick();
}

void Wanderer::OnClose() {
  Engine::OnClose();
}

}

std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::make_unique<wanderer::Wanderer>(); }