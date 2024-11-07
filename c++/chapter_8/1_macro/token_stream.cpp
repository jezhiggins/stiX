#include "token_stream.hpp"
#include "end_of_input.hpp"

stiX::token_stream::token_stream(std::istream& in):
  tokenizer_(in),
  tok_(tokenizer_.begin()) {
}

bool stiX::token_stream::token_available() const {
  return tok_ != tokenizer_.end();
}


std::string const& stiX::token_stream::peek_token() const {
  return token_available() ? *tok_ : EndOfInput;
}

std::string stiX::token_stream::pop_token() {
  auto tok = *tok_;
  ++tok_;
  return tok;
}
