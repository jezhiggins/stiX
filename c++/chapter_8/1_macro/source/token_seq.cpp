#include "token_seq.hpp"
#include <algorithm>

stiX::token_seq& stiX::token_seq::operator+=(std::string const& tok) {
  seq_.push_back(tok);
  return *this;
}
stiX::token_seq& stiX::token_seq::operator+=(std::string&& tok) {
  seq_.push_back(std::move(tok));
  return *this;
}
stiX::token_seq& stiX::token_seq::operator+=(stiX::token_seq&& tok) {
  std::ranges::copy(tok, std::back_inserter(seq_));
  return *this;
}
