/**
 * \file main.cpp
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#include <bloom.hpp>

/**
 *  Main class for the game
 */
class Sandbox : public bloom::Engine {
public:
  Sandbox() = default;
};

std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::make_unique<Sandbox>(); }
