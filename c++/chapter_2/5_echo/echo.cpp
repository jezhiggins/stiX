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

    template<typename InputRange, typename OutputIt, typename Separator = std::string>
    OutputIt join(InputRange&& range, OutputIt out, Separator sep = " ") {
      return join(std::begin(range), std::end(range), out, sep);
    }

    void echo(int argc, char const* argv[], std::ostream &out) {
        auto arguments = make_arguments(argc, argv);
        join(
            arguments,
            std::ostream_iterator<std::string>(out)
        );
    }
}
