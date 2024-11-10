#include "token_stream.hpp"
#include "end_of_input.hpp"
#include "../../../lib/chars.hpp"
#include <istream>
#include <ranges>

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

  char peek(std::vector<char> const& buf) {
    return buf.back();
  }

  char get(std::vector<char>& buf) {
    auto c = buf.back();
    buf.pop_back();
    return c;
  }

  bool input_available(std::vector<char> const& buf) {
    return !buf.empty();
  }

  char peek(std::vector<char> const& buf, std::istream* in) {
    return input_available(buf) ? peek(buf) : peek(in);
  }

  char get(std::vector<char>& buf, std::istream* in) {
    return input_available(buf) ? get(buf) : get(in);
  }

  bool input_available(std::vector<char> const& buf, std::istream* in) {
    return input_available(buf) || input_available(in);
  }
}

stiX::token_stream::token_stream(std::istream& in) :
    in_(&in) {
}

stiX::token_stream::token_stream(token_seq const& tokens) :
    in_(0) {
  push_tokens(tokens);
}

bool stiX::token_stream::token_available() {
  if (token_.empty())
    next_token();
  return !token_.empty();
}

std::string const& stiX::token_stream::peek_token() {
  return token_available() ? token_ : EndOfInput;
}

std::string stiX::token_stream::pop_token() {
  auto tok = peek_token();
  token_.clear();
  return tok;
}

void stiX::token_stream::push_token(std::string const& token) {
  if (!token_.empty()) {
    push_back(token_);
    token_.clear();
  }
  push_back(token);
}

void stiX::token_stream::push_tokens(token_seq const& tokens) {
  for(auto const& token : std::ranges::reverse_view(tokens))
    push_token(token);
}

void stiX::token_stream::push_back(std::string const& token) {
  std::ranges::copy(
    std::ranges::reverse_view(token),
    std::back_inserter(buffer_));
}

void stiX::token_stream::next_token() {
  if (input_available(buffer_, in_) && !stiX::isalnum(peek(buffer_, in_))) {
    token_ = std::string { get(buffer_, in_) };
    return;
  }

  token_.clear();
  if (input_available(buffer_, in_) && stiX::isdigit(peek(buffer_, in_)))
    while (input_available(buffer_, in_) && stiX::isdigit(peek(buffer_, in_)))
      token_ += get(buffer_, in_);
  else
    while (input_available(buffer_, in_) && stiX::isalnum(peek(buffer_, in_)))
      token_ += get(buffer_, in_);
}