#ifndef STICPP_PRINT_HPP
#define STICPP_PRINT_HPP

#include <iosfwd>
#include <string>

namespace stiX {
    int print(
        std::string filename,
        std::istream& input,
        std::ostream& output,
        size_t pageLength = 64
    );
}

#endif