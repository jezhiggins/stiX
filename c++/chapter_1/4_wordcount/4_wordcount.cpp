//
// wordcount
//
// wordcount counts the lines in its input and writes the total
// as line of text to the output. A "word" is a maximal sequence
// of characters not containing a blank, a tab, or a newline
//

#include "wordcount.hpp"
#include <iostream>

int main() {
    std::cout << stiX::wordcount(std::cin) << std::endl;
}
