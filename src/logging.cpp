#include "logging.hpp"

#include <cstdarg>
#include <mutex>
#include <raylib.h>

namespace urbis::logging {
std::shared_ptr<spdlog::sinks::dist_sink_mt> dist_sink = nullptr;
} // namespace urbis::logging

static std::once_flag _configure_logging_impl;
void configure_logging_impl() {
#ifdef NDEBUG
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::trace);
#endif
  spdlog::set_pattern(
      "[%Y-%m-%d %H:%M:%S.%e] [%n] [%^%l%$] [%s:%#] <%t> %v");
}
void urbis::logging::configure_logging() {
  std::call_once(_configure_logging_impl, configure_logging_impl);
}

static std::shared_ptr<spdlog::logger> raylib_logger = nullptr;

void raylib_callback(int log_level, const char *text, va_list args) {
  if (raylib_logger == nullptr)
    return;

  va_list args_cpy;
  va_copy(args_cpy, args);

  int bufsize = std::vsnprintf(nullptr, 0, text, args) + 1;
  char *buf = reinterpret_cast<char *>(std::malloc(sizeof(char) * bufsize));
  std::vsnprintf(buf, bufsize, text, args_cpy);
  switch (log_level) {
  case LOG_FATAL:
    raylib_logger->critical(buf);
    break;
  case LOG_ERROR:
    raylib_logger->error(buf);
    break;
  case LOG_WARNING:
    raylib_logger->warn(buf);
    break;
  case LOG_INFO:
    raylib_logger->info(buf);
    break;
  case LOG_DEBUG:
    raylib_logger->debug(buf);
    break;
  case LOG_TRACE:
    raylib_logger->debug(buf);
    break;
  default:
    break;
  }
  std::free(buf);
}

bool urbis::logging::set_raylib_callback() {
  raylib_logger =
      std::make_shared<spdlog::logger>("raylib", urbis::logging::dist_sink);
  if (raylib_logger == nullptr)
    LERR("Failed to create raylib logger");

  raylib_logger->set_level(spdlog::level::level_enum::trace);
  SetTraceLogCallback(raylib_callback);

  return true;
}
