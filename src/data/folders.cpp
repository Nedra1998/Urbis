#include "folders.hpp"

#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <string>

#ifndef _WIN32
#include <pwd.h>
#include <unistd.h>
#endif

urbis::data::PlatformFolders::PlatformFolders(const std::string &package)
    : data(get_data(package)), config(get_config(package)),
      cache(get_cache(package)) {}
urbis::data::PlatformFolders::~PlatformFolders() {}

#if defined(__unix__)
static std::string home() {
  int uid = getuid();
  const char *home_env = std::getenv("HOME");
  if (uid != 0 && home_env != nullptr)
    return std::string(home_env);

  struct passwd *pw = nullptr;
  struct passwd pwd;
  long bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
  if (bufsize < 0)
    bufsize = 16384L;
  char *buf = (char *)malloc(bufsize);
  int error_code = getpwuid_r(uid, &pwd, buf, bufsize, &pw);
  free(buf);
  if (pw == nullptr || error_code)
    throw std::runtime_error("Unable to get passwd struct.");
  if (!pw->pw_dir)
    throw std::runtime_error("User has no home directory");
  return std::string(pw->pw_dir);
}
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#elif defined(__APPLE__)
#elif defined(__unix__)
static std::string linux_folder_default(const char *env_name,
                                        const char *default_path) {
  const char *env_val = std::getenv(env_name);
  if (env_val)
    return std::string(env_val);
  return home() + '/' + default_path;
}
#endif

std::string urbis::data::PlatformFolders::get_data(const std::string &package) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  static_assert(false, "PlatformFolders::data() unsuported platform");
#elif defined(__APPLE__)
  if (!package.empty())
    return home() + "/Library/Application Support" + '/' + package;
  return home() + "/Library/Application Support";
#elif defined(__unix__)
  if (!package.empty())
    return linux_folder_default("XDG_DATA_HOME", ".local/share") + '/' +
           package;
  return linux_folder_default("XDG_DATA_HOME", ".local/share");
#else
  static_assert(false, "PlatformFolders::data() unsuported platform");
#endif
}

std::string
urbis::data::PlatformFolders::get_config(const std::string &package) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  static_assert(false, "PlatformFolders::config() unsuported platform");
#elif defined(__APPLE__)
  if (!package.empty())
    return home() + "/Library/Application Support" + '/' + package;
  return home() + "/Library/Application Support";
#elif defined(__unix__)
  if (!package.empty())
    return linux_folder_default("XDG_CONFIG_HOME", ".config") + '/' + package;
  return linux_folder_default("XDG_CONFIG_HOME", ".config");
#else
  static_assert(false, "PlatformFolders::config() unsuported platform");
#endif
}

std::string
urbis::data::PlatformFolders::get_cache(const std::string &package) {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  static_assert(false, "PlatformFolders::cache() unsuported platform");
#elif defined(__APPLE__)
  if (!package.empty())
    return home() + "/Library/Caches" + '/' + package;
  return home() + "/Library/Caches";
#elif defined(__unix__)
  if (!package.empty())
    return linux_folder_default("XDG_CACHE_HOME", ".cache") + '/' + package;
  return linux_folder_default("XDG_CACHE_HOME", ".cache");
#else
  static_assert(false, "PlatformFolders::cache() unsuported platform");
#endif
}
