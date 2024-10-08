#include "tokenizer.hpp"
#include "../../lib/chars.hpp"
#include <iostream>

constexpr auto eof = std::char_traits<char>::eof();

stiX::stream_token_iterator::stream_token_iterator(std::istream& input) :
  input_(&input) {
  token();
}
stiX::stream_token_iterator::stream_token_iterator() :
  input_() {
}

std::string const& stiX::stream_token_iterator::operator*() const {
  return token_;
}

stiX::stream_token_iterator& stiX::stream_token_iterator::operator++() {
  token();
  return *this;
}

void stiX::stream_token_iterator::token() {
  token_ = next_token();
} // token

std::string stiX::stream_token_iterator::next_token() {
  if(input_available() && !stiX::isalnum(peek()))
    return std::string { get() };

  std::string tok;
  while(input_available() && stiX::isalnum(peek()))
    tok += get();
  return tok;
} // next_token

bool stiX::stream_token_iterator::input_available() {
  if (!(input_ && input_->good() && peek() != eof))
    input_ = std::nullptr_t { };
  return input_;
}

char stiX::stream_token_iterator::peek() const {
  return static_cast<char>(input_->peek());
}

char stiX::stream_token_iterator::get() {
  return static_cast<char>(input_->get());
}

bool stiX::operator==(
    stiX::stream_token_iterator const& lhs,
    stiX::stream_token_iterator const& rhs) {
  return
    lhs.token_.empty() && !lhs.input_ &&
    rhs.token_.empty() && !rhs.input_;
}

/////////////
stiX::tokenizer::tokenizer(std::istream& input) :
  input_(input) {
}

stiX::stream_token_iterator stiX::tokenizer::begin() const {
  return stream_token_iterator(input_);
}

stiX::stream_token_iterator stiX::tokenizer::end() const {
  return stream_token_iterator();
}
