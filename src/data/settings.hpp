#ifndef URBIS_DATA_SETTINGS_HPP_
#define URBIS_DATA_SETTINGS_HPP_

#include <cstdlib>
#include <memory>

namespace urbis::data {
struct Settings {
  struct Video {
    std::size_t width, height;
    bool fullscreen;

    template <class Archive> void serialize(Archive &archive) {
      archive(width, height, fullscreen);
    }
  };

  struct Audio {
    float master, music, effects;

    template <class Archive> void serialize(Archive &archive) {
      archive(master, music, effects);
    }
  };

  Video video;
  Audio audio;

  template <class Archive> void serialize(Archive &archive) {
    archive(video, audio);
  }
};

extern std::shared_ptr<Settings> settings;

bool read_settings();
bool write_settings();

} // namespace urbis::data

#endif // URBIS_DATA_SETTINGS_HPP_