/**
 * @file key_event.hpp
 * @author Xein
 * @date 21/11/2024
 *
 * @brief Defines keyboard-related events in the Bloom Engine.
 *
 * This file contains classes for handling keyboard input events, including key presses
 * and key releases. These events derive from the base `KeyEvent` class and provide
 * additional context and utility methods for keyboard interaction.
 */

#pragma once
#include "event.hpp"
#include "bloom_header.hpp"

namespace bloom {

/**
 * @class KeyEvent
 * @brief Base class for all keyboard events.
 *
 * The `KeyEvent` class provides the common functionality for keyboard-related events,
 * such as retrieving the key code of the event. This class is designed to be extended
 * by specific event types, such as `KeyPressedEvent` and `KeyReleasedEvent`.
 */
class BLOOM_API KeyEvent : public Event {
public:
  /**
   * @brief Retrieves the key code associated with the event.
   * @return The key code as an integer.
   */
  [[nodiscard]] inline int GetKeyCode() const { return _keyCode; }

  EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
  /**
   * @brief Constructs a `KeyEvent`.
   * @param keycode The key code associated with the event.
   */
  explicit KeyEvent(const int keycode) : _keyCode(keycode) {}

  int _keyCode;  ///< The key code associated with the event.
};

/**
 * @class KeyPressedEvent
 * @brief Represents an event triggered when a key is pressed.
 *
 * The `KeyPressedEvent` class provides information about the key being pressed and
 * the number of times the key has been repeated while being held down.
 */
class BLOOM_API KeyPressedEvent : public KeyEvent {
public:
  /**
   * @brief Constructs a `KeyPressedEvent`.
   * @param keycode The key code associated with the event.
   * @param repeatCount The number of times the key press has been repeated.
   */
  KeyPressedEvent(const int keycode, const int repeatCount)
      : KeyEvent(keycode), _repeatCount(repeatCount) {}

  /**
   * @brief Retrieves the repeat count for the key press.
   * @return The number of times the key press has been repeated.
   */
  [[nodiscard]] int GetRepeatCount() const { return _repeatCount; }

  EVENT_CLASS_TYPE(KeyPressed)

  /**
   * @brief Provides a string representation of the `KeyPressedEvent`.
   * @return A string containing the event details.
   */
  [[nodiscard]] std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
    return ss.str();
  }

private:
  int _repeatCount;  ///< The number of times the key press has been repeated.
};

/**
 * @class KeyReleasedEvent
 * @brief Represents an event triggered when a key is released.
 * The `KeyReleasedEvent` class provides information about the key that has been released.
 */
class BLOOM_API KeyReleasedEvent : public KeyEvent {
public:
  /**
   * @brief Constructs a `KeyReleasedEvent`.
   * @param keycode The key code associated with the event.
   */
  KeyReleasedEvent(const int keycode) : KeyEvent(keycode) {}

  EVENT_CLASS_TYPE(KeyReleased)

  /**
   * @brief Provides a string representation of the `KeyReleasedEvent`.
   * @return A string containing the event details.
   */
  [[nodiscard]] std::string ToString() const override {
    std::stringstream ss;
    ss << "KeyReleasedEvent: " << _keyCode;
    return ss.str();
  }
};

}
