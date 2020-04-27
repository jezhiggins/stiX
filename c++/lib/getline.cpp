#include "getline.hpp"
#include <iostream>

std::string stiX::getline(std::istream& input) {
  std::string line;
  std::getline(input, line);
  return line;
} // getline

