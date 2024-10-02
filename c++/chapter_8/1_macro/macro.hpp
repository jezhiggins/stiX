#ifndef STICPP_MACRO_HPP
#define STICPP_MACRO_HPP

#include <iosfwd>
#include "tokenizer.hpp"

namespace stiX {
    void macro_process(std::istream& in, std::ostream& out);
}

#endif // STICPP_MACRO_HPP