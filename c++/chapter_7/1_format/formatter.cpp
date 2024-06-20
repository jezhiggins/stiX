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

  bool is_command(std::string const& line) {
    return !line.empty() && line.front() == '.';
  }
}

stiX::screen_formatter::screen_formatter(std::istream& in, std::ostream &out) :
  in_(in),
  out_(out),
  line_(0),
  max_width_(60),
  max_lines_(66),
  fill_(true) {
}

void stiX::screen_formatter::format() {
  while (in_.peek() != eof) {
    auto line = stiX::getline(in_);

    if (is_command(line))
      handle_command(line);
    else
      handle_text(line);
  }

  page_end();
}

////////////////////
void stiX::screen_formatter::handle_command(std::string const& line) {
  if (line == ".nf") {
    flush();
    fill_ = false;
  }
  if (line == ".fi") {
    flush();
    fill_ = true;
  }
}

void stiX::screen_formatter::handle_text(std::string const& line) {
  buffer_ += buffer_.empty() ? null : space;
  buffer_ += line;

  if (fill_)
    flush_if_wraps();
  else
    flush();
}

void stiX::screen_formatter::flush_if_wraps() {
  while (buffer_.length() > max_width_) {
    auto break_at = buffer_.rfind(' ', max_width_);

    line(fill_line(buffer_.substr(0, break_at), max_width_));

    buffer_ = buffer_.substr(break_at + 1);
  }
}

void stiX::screen_formatter::flush() {
  if (buffer_.empty())
    return;

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