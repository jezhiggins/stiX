#include "table.hpp"
#include <iostream>
#include <string>
#include <stdexcept>
#include "../../lib/getline.hpp"

namespace stiX {
  struct archive_file {
    std::string name;
    size_t filesize;
  };

  archive_file parse_header(std::string const& header) {
    if (header.find("-h- ") != 0)
      throw std::logic_error("Not a header line");

    auto lastspace = header.find_last_of(' ');
    auto namelength = lastspace-4;
    auto name = header.substr(4, namelength);
    auto size = header.substr(lastspace+1);

    return { name, std::stoul(size) };
  } // parse_header

  void table_archive(
    std::istream& archive,
    std::ostream& out
  ) {
    archive.peek();

    while (archive && !archive.eof()) {
      auto header_line = getline(archive);
      auto header = parse_header(header_line);

      out << header.name << '\t' << header.filesize << '\n';

      archive.peek();
    }
  } // table_archive
}
