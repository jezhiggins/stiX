#include "echo.h"

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

namespace stiX {
    std::vector<std::string> make_arguments(int argc, char const* argv[]) {
        std::vector<std::string> args;
        for (int a = 0; a != argc; ++a) 
            args.emplace_back(argv[a+1]);
        return args;
    }

    template<typename InputIt, typename OutputIt, typename Separator = std::string>
    OutputIt join(InputIt first, InputIt last, OutputIt d_first, Separator sep = " ") {
        if (first != last)
            *d_first++ = *first++;        
        while (first != last) {
            *d_first++ = sep;
            *d_first++ = *first++;
        }
        return d_first;
    }

    void echo(int argc, char const* argv[], std::ostream &out) {
        auto arguments = make_arguments(argc, argv);
        join(
            arguments.begin(), 
            arguments.end(), 
            std::ostream_iterator<std::string>(out)
        );
    }
}
