//
// overstrike
//
// overstrike copies its input to its output, replacing lines
// containing backspaces by multiple lines that overstrike
// to print the same as the input, but contain no backspaces.
// It is assumed that the output is to be printed on a device
// that takes the first character of each line as a carriage
// control; a blank carriage control causes normal space
// before print, while a plus sign '+' suppresses space
// before print and causes the remainder of the line to
// overstrike the previous line.

#include "overstrike.hpp"
#include <iostream>

int main() {
    stiX::overstrike(std::cin, std::cout);
}
