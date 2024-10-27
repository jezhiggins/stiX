#ifndef STICPP_TOKEN_BUFFER_HPP
#define STICPP_TOKEN_BUFFER_HPP

#include "token_seq.hpp"

namespace stiX {
  class token_buffer {
  public:
    token_buffer() = default;
    explicit token_buffer(token_seq const& tokens):
      buf_(tokens) { }

    bool token_available() const {
      return !buf_.empty();
    }

    std::string const& peek_token() const {
      return buf_.front();
    }

    std::string pop_token();

    void push_tokens(token_seq const& tokens);

  private:
    token_seq buf_;
  };
}

#endif //STICPP_TOKEN_BUFFER_HPP
