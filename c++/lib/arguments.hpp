#ifndef STICPP_ARGUMENTS_HPP
#define STICPP_ARGUMENTS_HPP

#include <string>
#include <vector>

namespace stiX {
    std::vector<std::string> make_arguments(int argc, char const* argv[]);
}
#endif