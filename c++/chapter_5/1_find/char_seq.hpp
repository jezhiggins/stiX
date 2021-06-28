#ifndef STICPP_CHAR_SEQ_HPP
#define STICPP_CHAR_SEQ_HPP

#include <string>

namespace stiX {
  class character_sequence {
  public:
    explicit character_sequence(std::string input);

    bool is_bol() const;
    bool is_eol() const;
    bool available() const;

    char operator *() const;

    void advance();

  private:
    const std::string input_;
    size_t pos_;
  };
}

#endif //STICPP_CHAR_SEQ_HPP
