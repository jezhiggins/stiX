#ifndef STICPP_END_OF_INPUT_HPP
#define STICPP_END_OF_INPUT_HPP

#include <string>

namespace stiX {
  static std::string constexpr EndOfInput =
    std::string { 1, '\0' };
}
#endif //STICPP_END_OF_INPUT_HPP
