#ifndef STICPP_TOKEN_STREAM_HPP
#define STICPP_TOKEN_STREAM_HPP

#include <iosfwd>
#include <string>

namespace stiX {
  class token_stream {
  public:
    token_stream() = default;
    explicit token_stream(std::istream& in);

    bool token_available() const;

    std::string const& peek_token() const;

    std::string pop_token();

  private:
    void next_token();

    std::istream* in_;
    std::string token_;
  };
}

#endif //STICPP_TOKEN_STREAM_HPP
