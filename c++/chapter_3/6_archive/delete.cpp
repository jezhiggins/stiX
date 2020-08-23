#include "delete.hpp"
#include <iostream>

#include "../../lib/getline.hpp"
#include "./archive_file.hpp"

void stiX::delete_from_archive(
  std::istream& archive_in,
  std::vector<std::string> const& files_to_remove,
  std::ostream& out
) {
  read_archive(
    archive_in,
    [&files_to_remove, &out](
      std::istream& archive_in,
      stiX::archive_file const& header
    ) {
      if (of_interest(files_to_remove, header))
        skip_entry(archive_in, header);
      else {
        out << header;
        copy_contents(archive_in, header, out);
      }
    }
  );
} // delete_from_archive

