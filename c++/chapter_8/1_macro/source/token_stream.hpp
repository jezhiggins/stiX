#ifndef STICPP_TOKEN_STREAM_HPP
#define STICPP_TOKEN_STREAM_HPP

#include <iosfwd>
#include <string>
#include <vector>
#include "token_seq.hpp"

namespace stiX {
  class token_stream {
  public:
    token_stream() = default;
    explicit token_stream(std::istream& in);
    explicit token_stream(token_seq const& in);

    bool token_available();

    std::string const& peek_token();

    std::string pop_token();

    void push_token(std::string const& token);
    void push_tokens(token_seq const& tokens);

  private:
    void push_back(std::string const& token);
    void next_token();

    std::istream* in_;
    std::vector<char> buffer_;
    std::string token_;
  };
}

#endif //STICPP_TOKEN_STREAM_HPP
