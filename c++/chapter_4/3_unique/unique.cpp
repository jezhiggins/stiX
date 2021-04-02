#include "unique.h"
#include "../../lib/getline.hpp"
#include <iostream>

void stiX::unique(std::istream &in, std::ostream &out) {
  while(in) {
    out << stiX::getline(in) << (!in.eof() ? "\n" : "");
  }
}

