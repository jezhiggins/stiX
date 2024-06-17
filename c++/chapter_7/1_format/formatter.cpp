#include "formatter.hpp"
#include "format.hpp"

#include "../../lib/getline.hpp"

#include <sstream>
#include <string>
#include "format_functions.hpp"

using namespace std::string_literals;

namespace {
  constinit auto eof = std::char_traits<char>::eof();
  constinit auto null = std::string_view { };
  constinit auto space = std::string_view { " " };
}

stiX::screen_formatter::screen_formatter(std::istream& in, std::ostream &out) :
  in_(in),
  out_(out),
  line_(0),
  max_width_(60),
  max_lines_(66) {
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

  page_end();
}

////////////////////
void stiX::screen_formatter::flush_if_wraps() {
  while (buffer_.length() > max_width_) {
    auto break_at = buffer_.rfind(' ', max_width_);

    line(fill_line(buffer_.substr(0, break_at), max_width_));

    buffer_ = buffer_.substr(break_at + 1);
  }
}

void stiX::screen_formatter::flush() {
  line(buffer_);
  buffer_.clear();
}

void stiX::screen_formatter::line(std::string_view line) {
  out_ << line << '\n';

  if (++line_ == max_lines_)
    line_ = 0;
}

void stiX::screen_formatter::page_end() {
  flush();

  while (line_ != 0)
    line("");
}

///////////
void stiX::format(std::istream& in, std::ostream& out) {
  auto formatter = screen_formatter { in, out };
  formatter.format();
}