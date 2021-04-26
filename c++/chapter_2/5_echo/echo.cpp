#include "echo.h"
#include "../../lib/arguments.hpp"
#include "../../lib/join.hpp"

#include <iostream>
#include <iterator>

namespace stiX {
    void echo(int argc, char const* argv[], std::ostream &out) {
        auto arguments = make_arguments(argc, argv);
        join(
            arguments,
            std::ostream_iterator<std::string>(out)
        );
    }
}
