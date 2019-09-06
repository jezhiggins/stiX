//
// expand
//
// expand copies its input, which has presumably been encoded
// by compress, to its output, replacing code sequences ~mc by
// the repeated characters they stand for so that the text
// output exactly matches that which was originally encoded.
// The occurrence of the warning character ~ in the input
// means that the next character is a repetition counter; 'A'
// calls for one instance of the following character, 'B' calls
// for two, and so on up to 'Z'.

#include "expand.h"
#include <iostream>

int main() {
  stiX::expand(std::cin, std::cout);
}
