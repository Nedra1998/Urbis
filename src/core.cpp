#include "core.hpp"

#include <fmt/format.h>
#include <raylib.h>
#include <spdlog/common.h>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#if defined(__APPLE__) || defined(__unix__) || defined(__unix)
#include <unistd.h>
#endif

#include "data/settings.hpp"
#include "logging.hpp"
#include "version.hpp"

int urbis::main() {
  if (!urbis::logging::set_raylib_callback())
    return 1;

  if (!urbis::data::read_settings())
    return 1;

  std::uint32_t config_flags = 0;
  if (urbis::data::settings->video.vsync)
    config_flags |= FLAG_VSYNC_HINT;
  if (urbis::data::settings->video.fullscreen)
    config_flags |= FLAG_FULLSCREEN_MODE;
  SetConfigFlags(config_flags);

  InitWindow(urbis::data::settings->video.width,
             urbis::data::settings->video.height, "Urbis");
  SetTargetFPS(urbis::data::settings->video.fps);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Congrats! You created your first window!", 190, 200, 20,
             LIGHTGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
