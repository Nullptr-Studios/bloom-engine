#include "log.h"

namespace bloom {

std::shared_ptr<spdlog::logger> Log::_bloomLogger;
std::shared_ptr<spdlog::logger> Log::_gameLogger;

void Log::Init() {
  spdlog::set_pattern("%^[%T] %L:%n: %v%$");
  _bloomLogger = spdlog::stdout_color_mt("BLOOM");
  _bloomLogger->set_level(spdlog::level::trace);
  _gameLogger = spdlog::stdout_color_mt("GAME");
  _gameLogger->set_level(spdlog::level::trace);
}

} // namespace bloom
