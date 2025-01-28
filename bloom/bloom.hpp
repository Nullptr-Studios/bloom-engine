/**
 * @file bloom.hpp
 * @author Xein 
 * @date 20/11/2024
 *
 * @brief DLL Wrapper for our game engine
 */

#pragma once

// ---Entry point------------------
#include "src/entrypoint.hpp"
// --------------------------------

#include "src/core.hpp"
#include "src/factory.hpp"
#include "src/log.hpp"
#include "src/layers/layer.hpp"
// ---Events-----------------------
#include "src/events/event.hpp"
#include "src/events/game_event.hpp"
#include "src/events/key_event.hpp"
#include "src/events/mouse_event.hpp"
// ---Objects----------------------
#include "src/objects/object.hpp"
#include "src/objects/actor.hpp"
#include "src/objects/camera.hpp"
#include "src/objects/directional_light.hpp"
#include "src/objects/point_light.hpp"
#include "src/objects/light.hpp"
#include "src/render/model.hpp"
#include "src/render/texture.hpp"
