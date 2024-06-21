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
  auto command = line.substr(0, 3);

  if (command == ".nf")
    nf_no_fill();
  if (command == ".fi")
    fi_fill_on();
  if (command == ".br")
    flush();
}

void stiX::screen_formatter::handle_text(std::string const& line) {
  if (line.empty()) {
    blank_line();
    return;
  }

  if (!fill_)
    line_print(line);
  else
    flush_if_wraps(line);
}

void stiX::screen_formatter::flush_if_wraps(std::string const& line) {
  buffer_ += (buffer_.empty() || line.empty()) ? null : space;
  buffer_ += line;

  while (buffer_.length() > max_width_) {
    auto break_at = buffer_.rfind(' ', max_width_);

    line_print(fill_line(buffer_.substr(0, break_at), max_width_));

    buffer_ = buffer_.substr(break_at + 1);
  }
}

void stiX::screen_formatter::flush() {
  if (buffer_.empty())
    return;

  line_print(buffer_);
  buffer_.clear();
}

void stiX::screen_formatter::blank_line() {
  flush();
  line_print("");
}

void stiX::screen_formatter::line_print(std::string_view line) {
  out_ << line << '\n';

  if (++line_ == max_lines_)
    line_ = 0;
}

void stiX::screen_formatter::page_end() {
  flush();

  while (line_ != 0)
    line_print("");
}

///////////
void stiX::screen_formatter::nf_no_fill() {
  set_fill_mode(false);
}
void stiX::screen_formatter::fi_fill_on() {
  set_fill_mode(true);
}
void stiX::screen_formatter::set_fill_mode(bool on) {
  flush();
  fill_ = on;
}

///////////
void stiX::format(std::istream& in, std::ostream& out) {
  auto formatter = screen_formatter { in, out };
  formatter.format();
}