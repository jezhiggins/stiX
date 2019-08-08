//
// entab
//
// entab copies its input to its output, replacing strings
// blanks by tabs so that the output is visually the same as
// the input, but contains fewer characters. Tab stops are
// assumed to be set every four columns (i.e. 1, 5, 9, ...),
// so that each sequence of one to foud blanks ending on a
// tab stop is replaced by a tab character.
//

#include "entab.h"
#include <iostream>

int main() {
    stiX::entab(std::cin, std::cout);
}
