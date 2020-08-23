#include "file_open.hpp"
#include <filesystem>

namespace fs = std::filesystem;

namespace stiX {
    std::ifstream file_opener(std::string const& filename) {
        return std::ifstream(filename);
    } // file_opener

    std::ofstream file_write_opener(std::string const& filename) {
      auto path = fs::path(filename);
      fs::create_directories(path.parent_path());

      return std::ofstream(path);
    } // file_write_opener
}
