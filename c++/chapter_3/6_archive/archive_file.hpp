#ifndef STICPP_ARCHIVE_FILE_HPP
#define STICPP_ARCHIVE_FILE_HPP

#include <string>
#include <iosfwd>

namespace stiX {
  struct archive_file {
    std::string name;
    size_t filesize;
  };

  archive_file parse_header(std::string const& header);
} // namespace stiX

std::ostream& operator<<(std::ostream& os, stiX::archive_file const& af);

#endif