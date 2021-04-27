//
// compress
//
// compress copies its input to its output, replacing
// strings of four or more identical characters by a code
// sequence so that the output generally contains fewer
// characters than the input. A run of x's is encoded as ~nx,
// where the count n is a character: 'A' calls for a
// repetition of one x, 'B" a repetition of two x's, and so
// on. Runs longer than 26 are broken into several shorter
// ones. Runs of ~'s of any length are encoded.

#include "compress.hpp"
#include <iostream>

int main() {
    stiX::compress(std::cin, std::cout);
}
