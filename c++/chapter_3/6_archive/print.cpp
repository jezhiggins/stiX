#include "./print.hpp"
#include <iostream>

#include "../../lib/getline.hpp"
#include "./archive_file.hpp"

namespace stiX {
  void print_files(
    std::istream& archive_in,
    std::vector<std::string> const& files,
    std::ostream& out
  ) {
    archive_in.peek();

    while(archive_in && !archive_in.eof()) {
      auto header_line = getline(archive_in);
      auto header = parse_header(header_line);

      if (of_interest(files, header))
        copy_contents(archive_in, header, out);
      else
        skip_entry(archive_in, header);

      archive_in.peek();
    } // while ...
  } // print_files
} // namespace stiX