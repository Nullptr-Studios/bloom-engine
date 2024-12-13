/**
 * @file game_event.hpp
 * @author Xein
 * @date 21/11/2024
 *
 * @brief Defines game-specific events in the Bloom Engine.
 *
 * This file provides classes for game-specific events, such as window resize, window close,
 * game ticks, and game rendering. These events derive from the base `Event` class and define
 * specific functionality for handling various scenarios encountered during the game's lifecycle.
 */

#pragma once
#include "event.hpp"
#include "bloom_header.hpp"

namespace bloom {

/**
 * @class WindowResizeEvent
 * @brief Represents an event triggered when the application window is resized.
 *
 * This event is dispatched when the window's width or height changes. It contains information
 * about the new dimensions of the window and provides a string representation for debugging.
 */
class BLOOM_API WindowResizeEvent : public Event {
public:
  /**
   * @brief Constructs a `WindowResizeEvent`.
   * @param width The new width of the window.
   * @param height The new height of the window.
   */
  WindowResizeEvent(unsigned int width, unsigned int height)
      : _width(width), _height(height) {}

  /**
   * @brief Retrieves the width of the window after resizing.
   * @return The width of the window.
   */
  unsigned int GetWidth() const { return _width; }

  /**
   * @brief Retrieves the height of the window after resizing.
   * @return The height of the window.
   */
  unsigned int GetHeight() const { return _height; }

  /**
   * @brief Overrides the `ToString` function to provide a detailed string representation of the event.
   * @return A string containing the event details.
   */
  std::string ToString() const override {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << _width << ", " << _height;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryGame)

private:
  unsigned int _width;  ///< The new width of the window.
  unsigned int _height; ///< The new height of the window.
};

/**
 * @class WindowCloseEvent
 * @brief Represents an event triggered when the application window is closed.
 *
 * This event is dispatched when the user or system initiates a window close action.
 */
class BLOOM_API WindowCloseEvent : public Event {
public:
  /**
   * @brief Constructs a `WindowCloseEvent`.
   */
  WindowCloseEvent() {}

  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryGame)
};

/**
 * @class GameTickEvent
 * @brief Represents an event triggered during the game update phase.
 *
 * This event is dispatched every time the game performs an update tick, which is used for
 * logic updates, such as moving objects or processing game mechanics.
 */
class BLOOM_API GameTickEvent : public Event {
public:
  /**
   * @brief Constructs a `GameTickEvent`.
   */
  GameTickEvent() {}

  EVENT_CLASS_TYPE(GameTick)
  EVENT_CLASS_CATEGORY(EventCategoryGame)
};

/**
 * @class GameRenderEvent
 * @brief Represents an event triggered during the game rendering phase.
 *
 * This event is dispatched every time the game renders a frame, which is used for drawing
 * the current state of the game to the screen.
 */
class BLOOM_API GameRenderEvent : public Event {
public:
  /**
   * @brief Constructs a `GameRenderEvent`.
   */
  GameRenderEvent() {}

  EVENT_CLASS_TYPE(GameRender)
  EVENT_CLASS_CATEGORY(EventCategoryGame)
};

}
