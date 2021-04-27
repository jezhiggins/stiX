//
// detab
//
// detab copies its input to its output, expanding horizontal
// tabs to blanks along the way, so that the output is visually
// the same as the input, but contains no tab characters. Tab
// stops are assumed to be set every four columns (i.e., 1,
// 5, 9, ...), so that each tab character is replaced by from
// one to four blanks.
//

#include "detab.hpp"
#include <iostream>

int main() {
    stiX::detab(std::cin, std::cout);
}
