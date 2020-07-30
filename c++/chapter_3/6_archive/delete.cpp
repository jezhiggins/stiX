#include "delete.hpp"
#include <iostream>
#include <string>
#include <algorithm>

#include "../../lib/getline.hpp"
#include "./archive_file.hpp"

void stiX::delete_from_archive(
  std::istream& archive_in,
  std::vector<std::string> const& files_to_remove,
  std::ostream& archive_out
) {
  archive_in.peek();

  while (archive_in && !archive_in.eof()) {
    auto header_line = getline(archive_in);
    auto header = parse_header(header_line);

    archive_out << header;

    std::copy_n(
      std::istreambuf_iterator<char>(archive_in),
      header.filesize,
      std::ostreambuf_iterator<char>(archive_out)
    );

    archive_in.get();
    archive_in.peek();
  } // while ...
} // delete_from_archive

