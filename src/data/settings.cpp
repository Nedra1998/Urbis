#include "data/settings.hpp"

#include <cereal/cereal.hpp>
#include <cereal/details/helpers.hpp>
#include <exception>
#include <filesystem>
#include <fstream>
#include <locale>
#include <new>
#include <string>
#include <system_error>
#include <utility>

#ifdef NDEBUG
#include <cereal/archives/portable_binary.hpp>
#else
#include <cereal/archives/json.hpp>
#endif

#include "folders.hpp"
#include "logging.hpp"

namespace urbis::data {
std::shared_ptr<Settings> settings = nullptr;
} // namespace urbis::data

bool urbis::data::read_settings() {
  PlatformFolders folders("urbis");
  std::filesystem::path config_file = folders.config / "config";
  settings = std::make_shared<Settings>();
  if (std::filesystem::exists(config_file)) {
    std::ifstream file(config_file);
    if (file.is_open()) {
      LDEBUG("Reading settings from (\"{}\").", config_file.string());
#ifdef NDEBUG
      cereal::PortableBinaryInputArchive iarchive(file);
#else
      cereal::JSONInputArchive iarchive(file);
#endif
      iarchive(*settings);
    } else {
      LWARN("Failed to open config file (\"{}\")for reading, ad default config "
            "will be used.",
            config_file.string());
    }

  } else {
    LINFO("No config file found (\"{}\"), a default config will be created.",
          config_file.string());
  }
  return true;
}

bool urbis::data::write_settings() {
  PlatformFolders folders("urbis");
  std::filesystem::path config_file = folders.config / "config";
  if (settings == nullptr) {
    LWARN("Setting have not been set, nothing to write");
    return false;
  }
  if (!std::filesystem::exists(folders.config)) {
    LDEBUG(
        "Package config directory (\"{}\") does not exist, it will be created.",
        folders.config.string());
    std::error_code err;
    if (!std::filesystem::create_directories(folders.config, err)) {
      LERR("Error {} ({}): {}", err.category().name(), err.value(),
           err.message());
      return false;
    }
  }
  std::ofstream file(config_file);
  if (file.is_open()) {
    LDEBUG("Writting settings to (\"{}\").", config_file.string());
#ifdef NDEBUG
    cereal::PortableBinaryOutputArchive oarchive(file);
#else
    cereal::JSONOutputArchive oarchive(file);
#endif
    oarchive(*settings);
  } else {
    LWARN("Failed to open config file (\"{}\")for writting.",
          config_file.string());
    return false;
  }
  return true;
}
