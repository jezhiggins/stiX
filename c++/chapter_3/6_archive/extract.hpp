#ifndef STICPP_EXTRACT_HPP
#define STICPP_EXTRACT_HPP

#include <string>
#include <vector>

#include "../../lib/file_open.hpp"
#include "./archive_file.hpp"

namespace stiX {
  void print_files(
    std::istream& archive_in,
    std::vector<std::string> const& files
  );

  void extract_files(
    std::istream& archive_in,
    std::vector<std::string> const& files
  );
}
#endif //STICPP_EXTRACT_HPP
