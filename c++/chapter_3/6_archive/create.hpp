#ifndef STICPP_ARCHIVE_CREATE_HPP
#define STICPP_ARCHIVE_CREATE_HPP

#include <iosfwd>
#include <string>
#include <vector>

namespace stiX {
  struct input_file {
    std::string const name;
    size_t const filesize;
  };

  void create_archive(
    std::vector<input_file> const& input,
    std::ostream& archive_out
  );
}

#endif