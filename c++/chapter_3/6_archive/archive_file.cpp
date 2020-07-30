#include "archive_file.hpp"
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

