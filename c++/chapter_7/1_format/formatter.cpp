#include "formatter.hpp"
#include "format.hpp"

#include "../../lib/getline.hpp"

#include <sstream>
#include <string>
#include <charconv>
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
stiX::screen_formatter::command_parameter stiX::screen_formatter::parse_value(
    std::string const& line,
    int def_value) const {
  auto type = value_type::Absolute;

  auto b = line.find_first_of("+-0123456789", 3);
  if (line[b] == '+' || line[b] == '-')
      type = value_type::Offset;
  b += (line[b] == '+');

  auto e = line.find_first_not_of("-0123456789", b);
  if (e == std::string::npos) e = line.size();

  auto value = def_value;
  std::from_chars(line.data() + b, line.data() + e, value);
  return { value, type };
}

void stiX::screen_formatter::handle_command(std::string const& line) {
  auto command = line.substr(0, 3);
  auto param = [&line, this](size_t def_value) {
    return parse_value(line, def_value);
  };

  if (command == ".nf")
    nf_no_fill();
  if (command == ".fi")
    fi_fill_on();
  if (command == ".br")
    flush();
  if (command == ".rm")
    set_right_margin(param(60));
  if (command == ".pl")
    set_page_length(param(66));
  if (command == ".sp")
    vertical_space(param(1));
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
void stiX::screen_formatter::vertical_space(command_parameter param) {
  flush();

  auto spaces = size_t{0};
  set_variable(spaces, param, 1, max_lines_ - line_);

  for (auto i = 0; i != spaces; ++i)
    line_print("");
}
void stiX::screen_formatter::set_right_margin(command_parameter param) {
  set_variable(max_width_, param);
}
void stiX::screen_formatter::set_page_length(command_parameter param) {
  set_variable(max_lines_, param);
}
void stiX::screen_formatter::set_variable(
  size_t& var,
  command_parameter update,
  size_t minimum,
  size_t maximum) {
  switch (update.type) {
    case value_type::Absolute:
      var = update.value;
      break;
    case value_type::Offset:
      var += update.value;
      break;
  }

  var = std::max(var, minimum);
  var = std::min(var, maximum);
}


///////////
void stiX::format(std::istream& in, std::ostream& out) {
  auto formatter = screen_formatter { in, out };
  formatter.format();
}