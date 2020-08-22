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
    read_archive(
      archive_in,
      [&files, &file_write_opener](
        std::istream& archive_in,
        stiX::archive_file const& header
      ) {
        if (of_interest(files, header)) {
          auto& out = file_write_opener(header.name);
          copy_contents(archive_in, header, out);
        }
        else
          skip_entry(archive_in, header);
      }
    );
  } // extract_archive
}
#endif //STICPP_EXTRACT_HPP
