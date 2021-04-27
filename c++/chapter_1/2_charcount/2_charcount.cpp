//
// charcount
//
// charcount counts the characters in its input and writes the total as a
// single line of text to the output. Since each line of text is internally
// delimited by a NEWLINE character, the total count is the number of lines
// plus the number of characters within each line.
//

#include "charcount.hpp"
#include <iostream>

int main() {
    std::cout << stiX::charcount(std::cin) << std::endl;
}



