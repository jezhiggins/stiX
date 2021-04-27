//
// sort
//
// sort sorts its input into ascending lexographic order.
// Two lines are in order if they are identical or if the
// leftmost character position in which they differ
// contains characters which are in order, using the
// internal numeric representation of the characters. If
// a line is a proper prefix of another line, it precedes
// that line in sort order.
//
// sort writes intermediate data to files in the temporary
// directory. They are deleted at program exit.
//
// To print the sorted output of a program
//   program | sort | print
//
#include "read_to_files.hpp"
#include "merge_files.hpp"
#include <iostream>

int main() {
  auto working_files = read_to_files(std::cin);

  merge_files(std::cout, working_files);
} // main

