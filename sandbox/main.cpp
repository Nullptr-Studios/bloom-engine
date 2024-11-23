/**
 * \file main.h
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#include <bloom.h>

/**
 *  Main class for the game
 */
class Sandbox : public bloom::Engine {
public:
  Sandbox() {
    _window = new bloom::Window(800, 600, "Bloom");
  };
  ~Sandbox() override = default;
};

std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::unique_ptr<Sandbox>(); }
