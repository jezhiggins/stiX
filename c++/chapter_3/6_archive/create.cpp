#include "create.hpp"

namespace stiX {
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
