#ifndef STICPP_ARCHIVE_CREATE_HPP
#define STICPP_ARCHIVE_CREATE_HPP

#include <iosfwd>
#include <string>

namespace stiX {
  struct input_file {
    std::string name;
    size_t filesize;
  };

  void create_archive(
    input_file input,
    std::ostream& archive_out
  );
}

#endif