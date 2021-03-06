//
// copy
//
// copy copies its input to its output unchanged. It is useful for copy from a
// terminal to a file, or even from a terminal to a terminal. It may be used for
// displaying the contents of a file, without interpretation or formatting, by
// copying from a file to a terminal.
//

#include "../../lib/copy.hpp"
#include <iostream>

int main() {
    stiX::copy(std::cin, std::cout);
}
