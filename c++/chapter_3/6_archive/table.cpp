#include "table.hpp"
#include <iostream>

#include "../../lib/getline.hpp"
#include "./archive_file.hpp"

namespace stiX {
  void table_archive(
    std::istream& archive_in,
    std::ostream& out
  ) {
    archive_in.peek();

    while (archive_in && !archive_in.eof()) {
      auto header_line = getline(archive_in);
      auto header = parse_header(header_line);

      out << header.name << '\t' << header.filesize << '\n';

      skip_entry(archive_in, header);

      archive_in.peek();
    }
  } // table_archive
}
