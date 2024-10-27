#ifndef STICPP_TOKEN_SOURCE_HPP
#define STICPP_TOKEN_SOURCE_HPP

#include "token_buffer.hpp"
#include "token_stream.hpp"

namespace stiX {
  class token_source {
  public:
    token_source() = default;
    explicit token_source(std::istream& is) :
      stream_(is) { }

    bool token_available() const {
      return buffer_.token_available() ||
        stream_.token_available();
    }

    std::string const& peek_token() const;

    std::string pop_token();

    void push_tokens(token_seq const& tokens) {
      buffer_.push_tokens(tokens);
    }

  private:
    token_buffer buffer_;
    token_stream stream_;
  };
}

#endif //STICPP_TOKEN_SOURCE_HPP