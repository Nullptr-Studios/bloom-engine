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
  Sandbox() = default;
  ~Sandbox() override = default;
};

bloom::Engine* bloom::CreateEngine() { return new Sandbox; }
