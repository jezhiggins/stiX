#ifndef STICPP_CHAR_SEQ_HPP
#define STICPP_CHAR_SEQ_HPP

#include <string>

namespace stiX {
  class character_sequence {
  public:
    explicit character_sequence(std::string input)
    {}

    bool is_bol() const;
    bool is_eol() const;
  };
}

#endif //STICPP_CHAR_SEQ_HPP
