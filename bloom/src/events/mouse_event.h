/**
 * @file mouse_event.h
 * @author Xein
 * @date 21/11/2024
 *
 * @brief Defines mouse-related events in the Bloom Engine.
 *
 * This file contains classes for handling mouse events, such as movement, scrolling,
 * and button interactions. Each event class provides specific details related to its
 * corresponding mouse action.
 */

#pragma once
#include "event.h"
#include <sstream>

namespace bloom {

/**
 * @class MouseMovedEvent
 * @brief Represents an event triggered when the mouse is moved.
 *
 * The `MouseMovedEvent` class captures the new position of the mouse cursor.
 */
class BLOOM_API MouseMovedEvent : public Event {
public:
  /**
   * @brief Constructs a `MouseMovedEvent`.
   * @param x The new X-coordinate of the mouse cursor.
   * @param y The new Y-coordinate of the mouse cursor.
   */
  MouseMovedEvent(const float x, const float y) : _mouseX(x), _mouseY(y) {}

  /**
   * @brief Retrieves the X-coordinate of the mouse cursor.
   * @return The X-coordinate as a float.
   */
  [[nodiscard]] float GetX() const { return _mouseX; }

  /**
   * @brief Retrieves the Y-coordinate of the mouse cursor.
   * @return The Y-coordinate as a float.
   */
  [[nodiscard]] float GetY() const { return _mouseY; }

  EVENT_CLASS_TYPE(MouseMoved)
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  /**
   * @brief Provides a string representation of the `MouseMovedEvent`.
   * @return A string containing the X and Y coordinates.
   */
  [[nodiscard]] std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseMovedEvent: " << _mouseX << ", " << _mouseY;
    return ss.str();
  }

private:
  float _mouseX, _mouseY;  ///< The X and Y coordinates of the mouse cursor.
};

/**
 * @class MouseScrolledEvent
 * @brief Represents an event triggered when the mouse wheel is scrolled.
 *
 * The `MouseScrolledEvent` class captures the offset values of the scroll action.
 */
class BLOOM_API MouseScrolledEvent : public Event {
public:
  /**
   * @brief Constructs a `MouseScrolledEvent`.
   * @param xOffset The horizontal scroll offset.
   * @param yOffset The vertical scroll offset.
   */
  MouseScrolledEvent(const float xOffset, const float yOffset)
      : _mouseXOffset(xOffset), _mouseYOffset(yOffset) {}

  /**
   * @brief Retrieves the horizontal scroll offset.
   * @return The X-offset as a float.
   */
  [[nodiscard]] float GetXOffset() const { return _mouseXOffset; }

  /**
   * @brief Retrieves the vertical scroll offset.
   * @return The Y-offset as a float.
   */
  [[nodiscard]] float GetYOffset() const { return _mouseYOffset; }

  EVENT_CLASS_TYPE(MouseScrolled)  ///< Declares the event type as `MouseScrolled`.
  EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

  /**
   * @brief Provides a string representation of the `MouseScrolledEvent`.
   * @return A string containing the scroll offsets.
   */
  [[nodiscard]] std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseScrolledEvent: " << _mouseXOffset << ", " << _mouseYOffset;
    return ss.str();
  }

private:
  float _mouseXOffset, _mouseYOffset;  ///< The horizontal and vertical scroll offsets.
};

/**
 * @class MouseButtonEvent
 * @brief Base class for mouse button events.
 *
 * The `MouseButtonEvent` class provides functionality for retrieving the button
 * involved in a mouse button event. This class is designed to be extended by
 * specific button event types, such as `MouseButtonPressedEvent` and `MouseButtonReleasedEvent`.
 */
class BLOOM_API MouseButtonEvent : public Event {
public:
  /**
   * @brief Retrieves the mouse button involved in the event.
   * @return The button as an integer.
   */
  [[nodiscard]] int GetMouseButton() const { return _button; }

  EVENT_CLASS_CATEGORY(EventCategoryMouseButton | EventCategoryMouse | EventCategoryInput)

protected:
  /**
   * @brief Constructs a `MouseButtonEvent`.
   * @param button The button involved in the event.
   */
  explicit MouseButtonEvent(int button) : _button(button) {}

  int _button;  ///< The mouse button involved in the event.
};

/**
 * @class MouseButtonPressedEvent
 * @brief Represents an event triggered when a mouse button is pressed.
 */
class BLOOM_API MouseButtonPressedEvent : public MouseButtonEvent {
public:
  /**
   * @brief Constructs a `MouseButtonPressedEvent`.
   * @param button The button that was pressed.
   */
  MouseButtonPressedEvent(const int button) : MouseButtonEvent(button) {}

  EVENT_CLASS_TYPE(MouseButtonPressed)

  /**
   * @brief Provides a string representation of the `MouseButtonPressedEvent`.
   * @return A string containing the button information.
   */
  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonPressedEvent: " << _button;
    return ss.str();
  }
};

/**
 * @class MouseButtonReleasedEvent
 * @brief Represents an event triggered when a mouse button is released.
 */
class BLOOM_API MouseButtonReleasedEvent : public MouseButtonEvent {
public:
  /**
   * @brief Constructs a `MouseButtonReleasedEvent`.
   * @param button The button that was released.
   */
  MouseButtonReleasedEvent(const int button) : MouseButtonEvent(button) {}

  EVENT_CLASS_TYPE(MouseButtonReleased)

  /**
   * @brief Provides a string representation of the `MouseButtonReleasedEvent`.
   * @return A string containing the button information.
   */
  std::string ToString() const override {
    std::stringstream ss;
    ss << "MouseButtonReleasedEvent: " << _button;
    return ss.str();
  }
};

}  // namespace bloom
