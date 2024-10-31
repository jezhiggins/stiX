#include "token_buffer.hpp"
#include "end_of_input.hpp"
#include <algorithm>
#include <ranges>

std::string const& stiX::token_buffer::peek_token() const {
  return token_available() ? buf_.front() : EndOfInput;
}

std::string stiX::token_buffer::pop_token() {
  auto tok = buf_.front();
  buf_.pop_front();
  return tok;
}

void stiX::token_buffer::push_token(
    const std::string &token) {
  buf_.push_front(token);
}
void stiX::token_buffer::push_tokens(
    stiX::token_seq const& tokens) {
  std::ranges::copy(
    std::ranges::reverse_view(tokens),
    std::front_inserter(buf_));
}
