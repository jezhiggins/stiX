
#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>

using namespace stiX;

void editor::process(std::istream &in, std::ostream &out) {
  while(in) {
    auto line = stiX::getline(in);

    if (line == "=")
      out << buffer_.dot() << "\n";
  }
}