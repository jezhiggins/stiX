#include "token_buffer.hpp"
#include <algorithm>
#include <ranges>

std::string stiX::token_buffer::pop_token() {
  auto tok = buf_.front();
  buf_.pop_front();
  return tok;
}

void stiX::token_buffer::push_tokens(
    stiX::token_seq const& tokens) {
  std::ranges::copy(
    std::ranges::reverse_view(tokens),
    std::front_inserter(buf_));
}
