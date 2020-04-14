#ifndef STICPP_ARGUMENTS_H
#define STICPP_ARGUMENTS_H

#include <string>
#include <vector>

namespace stiX {
    std::vector<std::string> make_arguments(int argc, char const* argv[]);
}
#endif