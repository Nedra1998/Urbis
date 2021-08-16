#ifndef URBIS_DATA_FOLDERS_HPP_
#define URBIS_DATA_FOLDERS_HPP_

#include <filesystem>
#include <string>

namespace urbis::data {
class PlatformFolders {
public:
  PlatformFolders(const std::string &package = std::string());
  ~PlatformFolders();

  std::filesystem::path data, config, cache;

protected:
  static std::string get_data(const std::string &package);
  static std::string get_config(const std::string &package);
  static std::string get_cache(const std::string &package);

private:
};
} // namespace urbis::data

#endif // URBIS_DATA_FOLDERS_HPP_