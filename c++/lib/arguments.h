#ifndef STICPP_ARGUMENTS_H
#define STICPP_ARGUMENTS_H

#include <string>
#include <vector>

namespace stiX {
    std::vector<std::string> make_arguments(int argc, char const* argv[]) {
        std::vector<std::string> args;
        for (int a = 1; a != argc; ++a)
            args.emplace_back(argv[a]);
        return args;
    }
}

#endif