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
  teapot->LoadTextures("resources/textures/cat.png");

  auto hardteapot = BLOOM_FACTORY->CreateObject<Actor>("HardTeapot");
  hardteapot->model = render::Model::LoadOBJ("resources/models/teapot.obj");
  hardteapot->transform.position = {-.50f, 0.0f, 0.0f};
  hardteapot->transform.scale = glm::vec3(0.5f, -0.5f, 0.5f);
  hardteapot->LoadTextures("resources/textures/bloom.png");

  auto floor = BLOOM_FACTORY->CreateObject<Actor>("Floor");
  floor->model = render::Model::LoadOBJ("resources/models/plane.obj");
  floor->transform.position = {0.0f, 0.0f, 0.0f};
  floor->transform.scale = glm::vec3(3.0f);
  floor->LoadTextures();

  auto light1 = BLOOM_FACTORY->CreateObject<PointLight>("Light1");
  light1->transform.position = {1.0f, -0.5f, 0.5f};
  light1->SetIntensity(.6f);
  light1->SetColor({1.0f, 1.0f, 0.0f});

  auto light2 = BLOOM_FACTORY->CreateObject<PointLight>("Light2");
  light2->transform.position = {-1.0f, -0.5f, 0.5f};
  light2->SetIntensity(.6f);
  light2->SetColor({1.0f, 0.0f, 1.0f});

  auto light3 = BLOOM_FACTORY->CreateObject<PointLight>("Light3");
  light3->transform.position = {0.0f, -0.5f, -0.5f};
  light3->SetIntensity(.6f);
  light3->SetColor({0.0f, 1.0f, 1.0f});
}

void Wanderer::OnTick() {
  Engine::OnTick();
}

void Wanderer::OnClose() {
  Engine::OnClose();
}

}

std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::make_unique<wanderer::Wanderer>(); }