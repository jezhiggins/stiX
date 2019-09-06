#include "echo.h"
#include "../../lib/arguments.h"

#include <algorithm>
#include <iostream>
#include <iterator>

namespace stiX {
    template<typename InputIt, typename OutputIt, typename Separator = std::string>
    OutputIt join(InputIt first, InputIt last, OutputIt dest, Separator sep = " ") {
        if (first != last)
            *dest++ = *first++;        
        while (first != last) {
            *dest++ = sep;
            *dest++ = *first++;
        }
        return dest;
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
