#include "engine.hpp"

namespace bloom {

Engine::Engine() { }

void Engine::Begin() {
  glfwInit();
	Log::Init();

  _window = new Window(800, 600, "Bloom");
  _window->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
  _window->OnInit();

  _devices = new render::Devices(*_window);
  _pipeline = new render::Pipeline(*_devices, "resources/shaders/default.vert.spv", "resources/shaders/default.frag.spv", render::Pipeline::defaultPipelineConfig(800, 600));
}

void Engine::Tick() {
  _window->OnTick();
}

void Engine::Render() {

}

void Engine::End() {
  delete _window;
}

void Engine::OnEvent(Event &e) {
  BLOOM_LOG("{0}", e.ToString());
}

} // namespace bloom
