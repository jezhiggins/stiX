
#include "editor.hpp"
#include "../../lib/getline.hpp"
#include <iostream>

using namespace stiX;

auto const eof = std::char_traits<char>::eof();

void editor::process(std::istream &in, std::ostream &out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    if (line == "=")
      out << buffer_.dot() << "\n";
    else
      out << "?\n";
  }
}