#include "token_stream.hpp"
#include "end_of_input.hpp"
#include "../../lib/chars.hpp"
#include <istream>

namespace {
  constexpr auto eof = std::char_traits<char>::eof();

  char peek(std::istream* in) {
    return static_cast<char>(in->peek());
  }

  char get(std::istream* in) {
    return static_cast<char>(in->get());
  }

  bool input_available(std::istream* in) {
    return in && in->good() && peek(in) != eof;
  }
}


stiX::token_stream::token_stream(std::istream& in):
  in_(&in) {
  next_token();
}

bool stiX::token_stream::token_available() const {
  return !token_.empty();
}

std::string const& stiX::token_stream::peek_token() const {
  return token_available() ? token_ : EndOfInput;
}

std::string stiX::token_stream::pop_token() {
  auto tok = token_;
  next_token();
  return tok;
}

void stiX::token_stream::next_token() {
  if (input_available(in_) && !stiX::isalnum(peek(in_))) {
    token_ = std::string { get(in_) };
    return;
  }

  token_.clear();
  if (input_available(in_) && stiX::isdigit(peek(in_)))
    while (input_available(in_) && stiX::isdigit(peek(in_)))
      token_ += get(in_);
  else
    while (input_available(in_) && stiX::isalnum(peek(in_)))
      token_ += get(in_);
}