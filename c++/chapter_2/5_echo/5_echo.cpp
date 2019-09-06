//
// echo
//
// echo copies its command line argments to its output as a
// line of text with one space between each argument. If
// there are no arguments, no output is produced,

#include "echo.h"
#include <iostream>

int main(int argc, char const* argv[]) {
    stiX::echo(argc, argv, std::cout);
    if (argc > 1)
        std::cout << '\n';
}
