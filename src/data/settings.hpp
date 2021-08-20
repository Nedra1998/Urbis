#ifndef URBIS_DATA_SETTINGS_HPP_
#define URBIS_DATA_SETTINGS_HPP_

#include <cereal/cereal.hpp>
#include <cinttypes>
#include <cstdlib>
#include <memory>

#include "logging.hpp"

namespace urbis::data {
struct Settings {
  struct Video {
    std::size_t width, height;
    bool fullscreen;

    template <class Archive>
    void serialize(Archive &archive, const std::uint32_t version) {
      switch (version) {
      case 0:
        archive(width, height, fullscreen);
        break;
      default:
        LERR("Unrecognized urbis::data::Settings::Video version ({})", version);
      }
    }
  };

  struct Audio {
    float master, music, effects;

    template <class Archive>
    void serialize(Archive &archive, const std::uint32_t version) {
      switch (version) {
      case 0:
        archive(master, music, effects);
        break;
      default:
        LERR("Unrecognized urbis::data::Settings::Audio version ({})", version);
      }
    }
  };

  Video video;
  Audio audio;

  template <class Archive>
  void serialize(Archive &archive, const std::uint32_t version) {
    switch (version) {
    case 0:
      archive(video, audio);
      break;
    default:
      LERR("Unrecognized urbis::data::Settings version ({})", version);
    }
  }
};

extern std::shared_ptr<Settings> settings;

bool read_settings();
bool write_settings();

} // namespace urbis::data

CEREAL_CLASS_VERSION(urbis::data::Settings::Video, 0);
CEREAL_CLASS_VERSION(urbis::data::Settings::Audio, 0);
CEREAL_CLASS_VERSION(urbis::data::Settings, 0);

#endif // URBIS_DATA_SETTINGS_HPP_
