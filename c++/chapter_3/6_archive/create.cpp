#include "create.hpp"

#include <iostream>

namespace stiX {
  void create_archive(
    input_file input,
    std::ostream& archive_out
  ) {
    if (input.name.empty())
      return;

    archive_out
        << "-h- "
        << input.name
        << ' '
        << input.filesize
        << "\n";
  } // create_archive
}