#include "unique.h"
#include "../../lib/getline.hpp"
#include <iostream>

auto const eof = std::char_traits<char>::eof();

void stiX::unique(std::istream &in, std::ostream &out) {
  auto previous_line = std::string { 1, '\0' };
  while(in.peek() != eof) {
    auto next_line = stiX::getline(in);
    if (previous_line != next_line) {
      out << next_line << "\n";
      previous_line = next_line;
    }
  };
}

