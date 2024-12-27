/**
 * \file core.hpp
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief Macro definitions for Bloom
 */

#pragma once

// DLL export declaration
#ifdef BLOOM_PLATFORM_WINDOWS
  #ifdef BLOOM_BUILD_DLL
    #define BLOOM_API __declspec(dllexport)
  #else
    #define BLOOM_API __declspec(dllimport)
  #endif

#else
  #error Bloom only supports windows!
  // TODO: Add support for other platforms
#endif

/**
 * @def BIT(x)
 * @brief Bit shifter macro for multi enums
 * @param x The bit to set
 */
#define BIT(x) (1 << x)

/**
 * @typedef id_t
 * @brief Typedef for object IDs
 */
typedef unsigned short id_t;

/**
 * @def DELETE_COPY_CONSTRUCTOR(classname)
 * @brief Macro to delete the copy constructor and assignment operator
 * @param classname The name of the class
 */
#define DELETE_COPY_CONSTRUCTOR(classname) \
  classname(const classname&) = delete; \
  classname& operator=(const classname&) = delete; \
  classname(classname&&) = default; \
  classname& operator=(classname&&) = default

#define BLOOM_FACTORY Factory::GetInstance()
#define BLOOM_RENDERER render::Renderer::GetInstance()
#define BLOOM_DEVICES render::Devices::GetInstance()

/**
 * @def EVENT_BIND(fn)
 * Binds a function to an event
 * @param fn Function to bind
 */
#define EVENT_BIND(fn) std::bind(&fn, this, std::placeholders::_1)

#define DRAG_SPEED(speed) auto io = ImgUi::GetIO(); return speed / io.KeyAlt ? 1.0f : 10.0f;
