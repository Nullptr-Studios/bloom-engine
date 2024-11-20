/**
 * \file core.h
 * \author Xein <xgonip@gmail.com>
 * \date 20/11/2024
 *
 * \brief [Brief description of the file's purpose]
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