#ifndef STICPP_INCLUDE_HPP
#define STICPP_INCLUDE_HPP

#include <iostream>
#include <string>

namespace stiX {
    std::string getline(std::istream& input);

    template<typename IncludeOpener>
    void expand_include(
        std::istream& input,
        std::ostream& output,
        IncludeOpener openFn
    ) {
        while(input) {
            std::string line = getline(input);

            output << line << (!input.eof() ? "\n" : "");
        } // while
    } // include
}
#endif //STICPP_INCLUDE_HPP
