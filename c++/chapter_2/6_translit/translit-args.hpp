#ifndef STICPP_TRANSLIT_ARGS_HPP
#define STICPP_TRANSLIT_ARGS_HPP

#include <string>

namespace stiX {
    bool isNegatedArgument(
        const std::string& arg
    );

    std::string translitArgument(
        const std::string& arg
    );
}

#endif //STICPP_TRANSLIT_ARGS_HPP
