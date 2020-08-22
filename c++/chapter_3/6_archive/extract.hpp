#ifndef STICPP_EXTRACT_HPP
#define STICPP_EXTRACT_HPP

#include <string>
#include <vector>

#include "./archive_file.hpp"

namespace stiX {
  template<typename FileWriteOpener>
  void extract_files(
    std::istream& archive_in,
    std::vector<std::string> const& files,
    FileWriteOpener file_write_opener
  ) {
  } // extract_archive
}
#endif //STICPP_EXTRACT_HPP
