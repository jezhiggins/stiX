//
// kwic  produce lines for KWIC index
//
// kwic writes one or more "folded" versions of each input
// line to its output. A line is "folded at the beginning
// of each alphanumeric string within the line by writing
// from that string through the end of the line, followed
// by the fold character $, followed by the beginning of
// the line.
// kwic is used with sort and unrotate to produce a KeyWord
// In Context, or KWIC, index.
//
// kwic
// This is a test.
// <ENDFILE>
// This is a test.$
// is a test.$This
// a test.$This is
// test.$This is a
//
// Normal usage is
// kwic < document | sort | unrotate
//
#include "kwic.hpp"
#include <iostream>

auto const fold_marker = '$';

int main() {
  stiX::kwic(std::cin, std::cout, fold_marker);
}