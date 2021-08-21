#include <catch2/catch_test_macros.hpp>
#include <cstdlib>
#include <data/folders.hpp>
#include <fmt/format.h>

using namespace urbis::data;

TEST_CASE("platform folders are correct", "[data]") {

  std::string user = std::getenv("USER");

  PlatformFolders folders("pkg");

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  SECTION("Windows") { FAIL(); }
#elif defined(__APPLE__)
  SECTION("Apple") { FAIL(); }
#elif defined(__unix__)
  SECTION("Unix") {
    REQUIRE(folders.data == fmt::format("/home/{}/.local/share/pkg", user));
    REQUIRE(folders.config == fmt::format("/home/{}/.config/pkg", user));
    REQUIRE(folders.cache == fmt::format("/home/{}/.cache/pkg", user));
  }
#else
  SECTION("Undefined") { FAIL(); }
#endif
}
