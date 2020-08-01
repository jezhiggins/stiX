#include "delete.hpp"
#include <iostream>

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

    if (of_interest(files_to_remove, header))
      skip_entry(archive_in, header);
    else {
      archive_out << header;
      copy_contents(archive_in, header, archive_out);
    }

    archive_in.peek();
  } // while ...
} // delete_from_archive

