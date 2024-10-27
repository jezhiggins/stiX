#include "token_stream.hpp"

std::string stiX::token_stream::pop_token() {
  auto tok = *tok_;
  ++tok_;
  return tok;
}
