#include "archive_file.hpp"
#include <iostream>
#include <stdexcept>

stiX::archive_file stiX::parse_header(std::string const& header) {
  if (header.find("-h- ") != 0)
    throw std::logic_error("Not a header line");

  auto lastspace = header.find_last_of(' ');
  auto namelength = lastspace-4;
  auto name = header.substr(4, namelength);
  auto size = header.substr(lastspace+1);

  return { name, std::stoul(size) };
} // parse_header

std::ostream& operator<<(std::ostream& os, stiX::archive_file const& af) {
  os << "-h- "
     << af.name
     << ' '
     << af.filesize
     << '\n';
  return os;
} // operator<<
