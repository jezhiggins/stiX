#include "token_stream.hpp"
#include "end_of_input.hpp"

std::string const& stiX::token_stream::peek_token() const {
  return token_available() ? *tok_ : EndOfInput;
}

std::string stiX::token_stream::pop_token() {
  auto tok = *tok_;
  ++tok_;
  return tok;
}
