#include "create.hpp"

#include <iostream>

namespace {
  void write_header(
    stiX::input_file const& input,
    std::ostream& archive_out
  );
} // namespace

namespace stiX {
  void create_archive(
    std::vector<input_file> const& input,
    std::ostream& archive_out
  ) {
    for (auto i : input)
      write_header(i, archive_out);
  } // create_archive
} // namespace stiX

namespace {
  void write_header(
    stiX::input_file const& input,
    std::ostream& archive_out
  ) {
    archive_out
      << "-h- "
      << input.name
      << ' '
      << input.filesize
      << "\n";
  } // write_header
}
