#include "archive_file.hpp"
#include <iostream>
#include <stdexcept>

std::string const HEADER_TAG = "-h- ";

stiX::archive_file stiX::parse_header(std::string const& header) {
  if (header.find(HEADER_TAG) != 0)
    throw std::logic_error("Not a header line");

  auto lastspace = header.find_last_of(' ');
  auto namelength = lastspace-4;
  auto name = header.substr(4, namelength);
  auto size = header.substr(lastspace+1);

  return { name, std::stoul(size) };
} // parse_header

std::ostream& operator<<(std::ostream& os, stiX::archive_file const& af) {
  os << HEADER_TAG
     << af.name
     << ' '
     << af.filesize
     << '\n';
  return os;
} // operator<<

void stiX::skip_entry(
  std::istream& archive_in,
  stiX::archive_file const& header
) {
  archive_in.seekg(header.filesize, std::ios_base::cur);
} // skip_entry
