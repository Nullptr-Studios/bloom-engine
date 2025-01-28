/**
 * @file log.hpp
 * @author Xein <xgonip@gmail.com>
 * @date 11/21/2024
 *
 * @brief Header file for the Log library class.
 *
 * This file declares the @c Log class, which provides a centralized logging system
 * for both the engine and the game. The logging functionality is built on top of
 * the @b spdlog library and supports multiple log levels (e.g., trace, info, warning, error).
 *
 * The file also includes the necessary macros and definitions for the Bloom and
 * Game logging consoles, ensuring clear separation between engine and game logs.
 */

#pragma once
#include "core.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace bloom {

/**
 * @class Log
 * @brief Centralized logging system for the engine and game.
 *
 * The @c Log class provides a unified interface for handling logging operations within the engine and game.
 * It acts as a wrapper around the @b spdlog library, offering convenient methods to log messages
 * at various severity levels (e.g., trace, info, warning, error, critical).
 *
 * This class manages two primary logging consoles:
 * - @b Bloom Console: Used for logging messages related to the engine's internal systems.
 * - @b Game Console: Used for logging messages specific to the game logic and runtime.
 *
 * @note The @c BLOOM_API macro is used to expose this class for external use in a shared library context.
 */
class BLOOM_API Log {

public:
  /**
   * @brief This function initializes the logger for the game
   */
  static void Init();

  /**
   * Gets the bloom log console
   * @return Bloom Log console shared pointer
   */
  inline static std::shared_ptr<spdlog::logger>& GetBloomLogger() { return _bloomLogger; }
    /**
     * Gets the game log console
     * @return Game Log console shared pointer
     */
  inline static std::shared_ptr<spdlog::logger>& GetGameLogger() { return _gameLogger; }

private:
  static std::shared_ptr<spdlog::logger> _bloomLogger;
  static std::shared_ptr<spdlog::logger> _gameLogger;
};

}

// Engine log macros

/**
 * @def BLOOM_CRITICAL(message)
 * @brief Logs a critical error message to the Bloom logger.
 *
 * This macro sends a critical-level log message using the Bloom logger.
 * Critical messages are the most severe and indicate fatal errors.
 *
 * @param ... The message and optional arguments to log.
 */
#define BLOOM_CRITICAL(...)   {::bloom::Log::GetBloomLogger()->critical(__VA_ARGS__);\
                              __debugbreak();\
                              std::exit(1);}

/**
 * @def BLOOM_ERROR(message)
 * @brief Logs an error message to the Bloom logger.
 *
 * This macro sends an error-level log message using the Bloom logger.
 * Error messages indicate a significant problem but may not terminate the application.
 *
 * @param ... The message and optional arguments to log.
 */
#define BLOOM_ERROR(...)      ::bloom::Log::GetBloomLogger()->error(__VA_ARGS__);\
                              __debugbreak()

/**
 * @def BLOOM_WARN(message)
 * @brief Logs a warning message to the Bloom logger.
 *
 * This macro sends a warning-level log message using the Bloom logger.
 * Warning messages indicate potential issues that might require attention.
 *
 * @param ... The message and optional arguments to log.
 */
#define BLOOM_WARN(...)       ::bloom::Log::GetBloomLogger()->warn(__VA_ARGS__)

/**
 * @def BLOOM_INFO(message)
 * @brief Logs an informational message to the Bloom logger.
 *
 * This macro sends an informational-level log message using the Bloom logger.
 * Informational messages are used to provide general details about application state.
 *
 * @param ... The message and optional arguments to log.
 */
#define BLOOM_INFO(...)       ::bloom::Log::GetBloomLogger()->info(__VA_ARGS__)

/**
 * @def BLOOM_LOG(message)
 * @brief Logs a trace/debug message to the Bloom logger.
 *
 * This macro sends a trace-level log message using the Bloom logger.
 * Trace messages are used for debugging purposes and provide detailed application flow.
 *
 * @param ... The message and optional arguments to log.
 */
#define BLOOM_LOG(...)        ::bloom::Log::GetBloomLogger()->trace(__VA_ARGS__)

// Game log macros

/**
 * @def GAME_CRITICAL(message)
 * @brief Logs a critical error message to the Game logger.
 *
 * This macro sends a critical-level log message using the Game logger.
 * Critical messages indicate fatal errors related to the game system.
 *
 * @param ... The message and optional arguments to log.
 */
#define GAME_CRITICAL(...)    ::bloom::Log::GetGameLogger()->critical(__VA_ARGS__)

/**
 * @def GAME_ERROR(message)
 * @brief Logs an error message to the Game logger.
 *
 * This macro sends an error-level log message using the Game logger.
 * Error messages indicate problems in the game system that require attention.
 *
 * @param ... The message and optional arguments to log.
 */
#define GAME_ERROR(...)       ::bloom::Log::GetGameLogger()->error(__VA_ARGS__)

/**
 * @def GAME_WARN(message)
 * @brief Logs a warning message to the Game logger.
 *
 * This macro sends a warning-level log message using the Game logger.
 * Warning messages indicate non-critical issues in the game system.
 *
 * @param ... The message and optional arguments to log.
 */
#define GAME_WARN(...)        ::bloom::Log::GetGameLogger()->warn(__VA_ARGS__)

/**
 * @def GAME_INFO(message)
 * @brief Logs an informational message to the Game logger.
 *
 * This macro sends an informational-level log message using the Game logger.
 * Informational messages provide general status updates about the game system.
 *
 * @param ... The message and optional arguments to log.
 */
#define GAME_INFO(...)        ::bloom::Log::GetGameLogger()->info(__VA_ARGS__)

/**
 * @def GAME_LOG(message)
 * @brief Logs a trace/debug message to the Game logger.
 *
 * This macro sends a trace-level log message using the Game logger.
 * Trace messages provide detailed debugging information for the game system.
 *
 * @param ... The message and optional arguments to log.
 */
#define GAME_LOG(...)         ::bloom::Log::GetGameLogger()->trace(__VA_ARGS__)

