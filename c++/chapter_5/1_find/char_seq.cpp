#include "char_seq.hpp"

stiX::character_sequence::character_sequence(std::string input)
  : input_(std::move(input)),
    pos_(0) {
}

bool stiX::character_sequence::is_bol() const {
  return pos_ == 0;
}

bool stiX::character_sequence::is_eol() const {
  return pos_ == input_.length();
}

char stiX::character_sequence::operator*() const {
  return input_[pos_];
}

void stiX::character_sequence::advance() {
  if (!is_eol()) ++pos_;
}


