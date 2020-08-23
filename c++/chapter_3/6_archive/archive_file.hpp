#ifndef STICPP_ARCHIVE_FILE_HPP
#define STICPP_ARCHIVE_FILE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include "../../lib/getline.hpp"

namespace stiX {
  struct archive_file {
    std::string name;
    size_t filesize;
  };

  archive_file parse_header(std::string const& header);

  void skip_entry(
    std::istream& archive_in,
    stiX::archive_file const& header
  );

  bool of_interest(
    std::vector<std::string> const& files_to_remove,
    stiX::archive_file const& name
  );

  void copy_contents(
    std::istream& archive_in,
    stiX::archive_file const& header,
    std::ostream& archive_out
  );

  typedef std::function<
    void(
      std::istream&,
      stiX::archive_file const&
    )
  > ArchiveReader;

  void read_archive(
    std::istream& archive_in,
    ArchiveReader reader
  );
} // namespace stiX

std::ostream& operator<<(std::ostream& os, stiX::archive_file const& af);

#endif