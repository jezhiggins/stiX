#include "read_to_files.hpp"
#include "merge_files.hpp"
#include <iostream>

int main() {
  auto working_files = read_to_files(std::cin);

  merge_files(std::cout, working_files);
} // main

