/**
 * \file core.hpp
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief Macro definitions for Bloom
 */

#pragma once

#ifdef BLOOM_PLATFORM_WINDOWS
  #ifdef BLOOM_BUILD_DLL
    #define BLOOM_API __declspec(dllexport)
  #else
    #define BLOOM_API __declspec(dllimport)
  #endif

#else
  #error Bloom only supports windows!
#endif

#define BIT(x) (1 << x)

enum megabool {
  FALSE, TRUE, NEITHER, BOTH,
  MAYBE, TRUEISH, FALSEISH,
  IT_DEPENDS, OSCILLATING,
  ITS_COMPLICATED,
  DOUBLE_FALSE, DOUBLE_TRUE,
  UNKNOWN, UNDEFINED
};