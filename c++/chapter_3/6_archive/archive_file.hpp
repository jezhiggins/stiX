#ifndef STICPP_ARCHIVE_FILE_HPP
#define STICPP_ARCHIVE_FILE_HPP

#include <string>

namespace stiX {
  struct archive_file {
    std::string name;
    size_t filesize;
  };
} // namespace stiX
#endif