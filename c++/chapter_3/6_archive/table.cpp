#include "table.hpp"
#include <iostream>

#include "../../lib/getline.hpp"
#include "./archive_file.hpp"

namespace stiX {
  void table_archive(
    std::istream& archive,
    std::ostream& out
  ) {
    archive.peek();

    while (archive && !archive.eof()) {
      auto header_line = getline(archive);
      auto header = parse_header(header_line);

      out << header.name << '\t' << header.filesize << '\n';

      archive.seekg(header.filesize, std::ios_base::cur);
      archive.peek();
    }
  } // table_archive
}
