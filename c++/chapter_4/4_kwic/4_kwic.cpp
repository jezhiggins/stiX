#include "kwic.hpp"
#include <iostream>

auto const fold_marker = '$';

int main() {
  stiX::kwic(std::cin, std::cout, fold_marker);
}