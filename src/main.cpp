#include <cxxopts.hpp>
#include <fmt/format.h>
#include <spdlog/common.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#endif

#include "data/settings.hpp"
#include "logging.hpp"
#include "version.hpp"

int main(int argc, char const *argv[]) {
  cxxopts::Options options("urbis",
                           fmt::format("Urbis - Colony Management Game - v{}",
                                       urbis::version::core));
  // clang-format off
  options.add_options()
      ("h,help", "print usage")
      ("v,verbose", "enable verbose logging", cxxopts::value<bool>()->default_value("false"))
      ("log-path", "log file path", cxxopts::value<std::string>());
  // clang-format on

  auto result = options.parse(argc, argv);
  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    return 0;
  }

  try {
    urbis::logging::dist_sink = std::make_shared<spdlog::sinks::dist_sink_mt>();
#if defined(__APPLE__) || defined(__unix__) || defined(__unix)
    if (::isatty(fileno(stdout)) != 0) {
      auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      sink->set_level(result["verbose"].as<bool>() ? spdlog::level::debug
                                                   : spdlog::level::warn);
      urbis::logging::dist_sink->add_sink(sink);
    } else {
      auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
      sink->set_level(result["verbose"].as<bool>() ? spdlog::level::debug
                                                   : spdlog::level::warn);
      urbis::logging::dist_sink->add_sink(sink);
    }
#else
    {
      auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
      sink->set_level(result["verbose"].as<bool>() ? spdlog::level::debug
                                                   : spdlog::level::warn);
      urbis::logging::dist_sink->add_sink(sink);
    }
#endif

    if (result.count("log-file")) {
      auto sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
          result["log-file"].as<std::string>(), 1024 * 1024, 5, false);
      sink->set_level(result["verbose"].as<bool>() ? spdlog::level::trace
                                                   : spdlog::level::debug);
      urbis::logging::dist_sink->add_sink(sink);
    } else {
      LWARN("No logfile set, file logger will not be initialized.");
    }
  } catch (const spdlog::spdlog_ex &ex) {
    std::cerr << "Log initialization failed: " << ex.what() << std::endl;
    return 1;
  }

  if (!urbis::data::read_settings())
    return 1;
  LINFO("Resolution {}x{}", urbis::data::settings->video.width,
        urbis::data::settings->video.height);
  if (!urbis::data::write_settings())
    return 1;

  return 0;
}
