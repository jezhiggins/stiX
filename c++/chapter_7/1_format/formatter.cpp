#include "formatter.hpp"
#include "format.hpp"

#include "../../lib/getline.hpp"

#include <sstream>
#include <string>

using namespace std::string_literals;

namespace {
  constinit auto eof = std::char_traits<char>::eof();
  constinit auto null = std::string_view { };
  constinit auto space = std::string_view { " " };
}

stiX::screen_formatter::screen_formatter(std::istream& in, std::ostream &out) :
  in_(in),
  out_(out),
  maxlen_(60) {
}

void stiX::screen_formatter::format() {
  auto separator = null;

  while (in_.peek() != eof) {
    auto line = stiX::getline(in_);

    buffer_ += separator;
    buffer_ += line;
    flush_if_wraps();

    separator = space;
  }

  flush();
}

void stiX::screen_formatter::flush_if_wraps() {
  while (buffer_.length() > maxlen_) {
    auto break_at = buffer_.rfind(' ', maxlen_);
    out_ << buffer_.substr(0, break_at) << '\n';
    buffer_ = buffer_.substr(break_at + 1);
  }
}

void stiX::screen_formatter::flush() {
  out_ << buffer_ << '\n';
  buffer_.clear();
}

///////////
void stiX::format(std::istream& in, std::ostream& out) {
  auto formatter = screen_formatter { in, out };
  formatter.format();
}