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

    void checkpoint();
    bool rewind();

  private:
    std::string const input_;
    size_t const input_length_;
    size_t pos_;
    size_t checkpoint_;
  };
}

#endif //STICPP_CHAR_SEQ_HPP
