#ifndef STICPP_ARCHIVE_CREATE_HPP
#define STICPP_ARCHIVE_CREATE_HPP

#include "../../lib/copy.hpp"
#include "../../lib/file_open.hpp"
#include <iosfwd>
#include <string>
#include <vector>
#include <iostream>
#include "./archive_file.hpp"

namespace stiX {
  template<typename FileReader>
  void write_contents(
    stiX::archive_file const& input,
    std::ostream& archive_out,
    FileReader fileReader
  ) {
    auto inputStream = fileReader(input.name);
    copy(inputStream, archive_out);
  } // write_contents

  template<typename FileReader = decltype(file_opener)>
  void append_archive(
    std::vector<archive_file> const& input,
    std::ostream& archive_out,
    FileReader fileReader = file_opener
  ) {
    for (auto i : input) {
      archive_out << i;
      write_contents(i, archive_out, fileReader);
    } // for ...
  } // append_archive
} // namespace stiX
#endif