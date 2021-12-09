#include "char_seq.hpp"

stiX::character_sequence::character_sequence(
  std::string_view input,
  size_type offset
) : input_(std::move(input)),
    input_length_(input_.size()),
    pos_(offset) {
}

bool stiX::character_sequence::is_bol() const {
  return pos_ == 0;
}

bool stiX::character_sequence::is_eol() const {
  return pos_ == input_length_;
}

bool stiX::character_sequence::available() const {
  return (input_length_ != 0) && (pos_ < (input_length_ - 1));
}

char stiX::character_sequence::operator*() const {
  return !is_eol() ? input_[pos_] : char(0);
}

void stiX::character_sequence::advance() {
  if (!is_eol()) ++pos_;
}

void stiX::character_sequence::checkpoint() {
  checkpoint_ = pos_;
}

bool stiX::character_sequence::rewind() {
  if (checkpoint_ == pos_)
    return false;

  --pos_;
  return true;
}


