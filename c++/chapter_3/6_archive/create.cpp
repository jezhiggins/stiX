#include "create.hpp"

#include <iostream>

namespace stiX {
  void create_archive(
    std::ostream& archive_out
  ) {

  } // create_archive

  void create_archive(
    input_file input,
    std::ostream& archive_out
  ) {
    archive_out << "-h- nothing 0\n";
  } // create_archive
}