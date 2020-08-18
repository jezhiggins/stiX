#ifndef STICPP_ARCHIVE_FILE_HPP
#define STICPP_ARCHIVE_FILE_HPP

#include <string>
#include <vector>
#include <iostream>
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

    template<typename ArchiveReader>
    void read_archive(
        std::istream& archive_in,
        ArchiveReader reader
    ) {
      archive_in.peek();

      while(archive_in && !archive_in.eof()) {
        auto header_line = getline(archive_in);
        auto header = parse_header(header_line);

        reader(archive_in, header);

        archive_in.peek();
      } // while ...
    } // read_archive
} // namespace stiX

std::ostream& operator<<(std::ostream& os, stiX::archive_file const& af);

#endif