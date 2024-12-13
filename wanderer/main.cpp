/**
 * \file main.cpp
 * \author Xein <xgonip@gmail.com>
 * \date 11/12/2024
 *
 * \brief [Brief description of the file's purpose]
 */

#include <bloom.hpp>

/**
 *  Main class for the game
 */
class Wanderer : public bloom::Engine {
public:
  Wanderer() = default;
};

std::unique_ptr<bloom::Engine> bloom::CreateEngine() { return std::make_unique<Wanderer>(); }
