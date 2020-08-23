#include "file_open.hpp"

namespace stiX {
    std::ifstream file_opener(std::string const& filename) {
        return std::ifstream(filename);
    } // file_opener

    std::ofstream file_write_opener(std::string const& filename) {
      return std::ofstream(filename);
    } // file_write_opener
}
