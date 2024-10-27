#include "token_source.hpp"
#include <stdexcept>

std::string const& stiX::token_source::peek_token() const {
  if (buffer_.token_available())
    return buffer_.peek_token();

  if (stream_.token_available())
    return stream_.peek_token();

  return EndOfInput;
}

std::string stiX::token_source::pop_token() {
  if (buffer_.token_available())
    return buffer_.pop_token();

  if (stream_.token_available())
    return stream_.pop_token();

  throw std::runtime_error("Unexpected end of input");
}
