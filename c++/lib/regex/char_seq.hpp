#ifndef STICPP_CHAR_SEQ_HPP
#define STICPP_CHAR_SEQ_HPP

#include <string>

namespace stiX {
  class character_sequence {
  public:
    using size_type = std::string_view::size_type;

    explicit character_sequence(std::string_view input);

    bool is_bol() const;
    bool is_eol() const;
    bool available() const;

    char operator *() const;

    void advance();

    void checkpoint();
    bool rewind();

    void mark_from() { from_ = pos_; }
    size_type from() const { return from_; }
    size_type to() const { return pos_; }
  private:
    std::string_view const input_;
    size_type const input_length_;
    size_type from_;
    size_type pos_;
    size_type checkpoint_;
  };
}

#endif //STICPP_CHAR_SEQ_HPP
