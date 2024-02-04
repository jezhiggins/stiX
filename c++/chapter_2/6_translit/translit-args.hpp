#ifndef STICPP_TRANSLIT_ARGS_HPP
#define STICPP_TRANSLIT_ARGS_HPP

#include <string>

namespace stiX {
    bool isNegatedArgument(
        std::string_view arg
    );

    std::string translitSrcArgument(
        std::string_view arg
    );

    std::string translitDestArgument(
        std::string_view arg
    );
}

#endif //STICPP_TRANSLIT_ARGS_HPP
