#ifndef STICPP_TOKEN_STREAM_HPP
#define STICPP_TOKEN_STREAM_HPP

#include "tokenizer.hpp"

namespace stiX {
  class token_stream {
  public:
    token_stream() = default;
    explicit token_stream(std::istream& in):
      tokenizer_(in),
      tok_(tokenizer_.begin()) {
    }

    bool token_available() const {
      return tok_ != tokenizer_.end();
    }

    std::string const& peek_token() const {
      return *tok_;
    }

    std::string pop_token();

  private:
    stiX::tokenizer tokenizer_;
    stiX::stream_token_iterator tok_;
  };
}

#endif //STICPP_TOKEN_STREAM_HPP
