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

  constinit auto default_right_margin = 60;
  constinit auto default_page_length = 66;
  constinit auto default_vertical_space = 1;
  constinit auto default_line_space = 1;
  constinit auto default_active_lines = 1;

  bool is_command(std::string const& line) {
    return !line.empty() && line.front() == '.';
  }
}

stiX::screen_formatter::screen_formatter(std::istream& in, std::ostream& out) :
  screen_formatter(in, out, default_right_margin, default_page_length) {
}

stiX::screen_formatter::screen_formatter(
    std::istream& in,
    std::ostream& out,
    size_t page_width,
    size_t page_length) :
  in_(in),
  out_(out),
  current_line_(0),
  right_margin_(page_width),
  page_length_(page_length),
  line_space_(default_line_space),
  centring_(0),
  underline_(0),
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
    set_right_margin(param(default_right_margin));
  if (command == ".pl")
    set_page_length(param(default_page_length));
  if (command == ".sp")
    vertical_space(param(default_vertical_space));
  if (command == ".ls")
    set_line_space(param(default_line_space));
  if (command == ".ce")
    set_centre(param(default_active_lines));
  if (command == ".ul")
    set_underline(param(default_active_lines));
}

void stiX::screen_formatter::handle_text(std::string line) {
  if (underline_) {
    --underline_;
    line = stiX::underline(line);
  }

  if (centring_) {
    --centring_;
    line_print(centre_line(line, right_margin_));
    return;
  }

  if (line.empty()) {
    blank_line();
    return;
  }

  if (!fill_)
    line_print(line);
  else
    line_buffer(line);
}

void stiX::screen_formatter::line_buffer(std::string const& line) {
  for (auto word: split_into_words(line)) {
    if (count_width(buffer_) + word.width >= right_margin_)
      fill_and_flush();

    buffer_ += buffer_.empty() ? null : space;
    buffer_ += word.word;
  }
}

void stiX::screen_formatter::fill_and_flush() {
  buffer_ = fill_line(buffer_, right_margin_);
  flush();
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
  out_ << line;

  auto line_space = std::min(line_space_, lines_remaining());
  for (auto i = 0; i != line_space; ++i)
    line_feed();
}
void stiX::screen_formatter::line_feed() {
  out_ << '\n';
  if (++current_line_ == page_length_)
    current_line_ = 0;
}

void stiX::screen_formatter::page_end() {
  flush();

  while (current_line_ != 0)
    line_feed();
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
  set_variable(spaces, param, 0, lines_remaining());

  for (auto i = 0; i != spaces; ++i)
    line_feed();
}
void stiX::screen_formatter::set_right_margin(command_parameter param) {
  set_variable(right_margin_, param);
}
void stiX::screen_formatter::set_page_length(command_parameter param) {
  set_variable(page_length_, param);
}
void stiX::screen_formatter::set_line_space(command_parameter param) {
  set_variable(line_space_, param);
}
void stiX::screen_formatter::set_centre(command_parameter param) {
  flush();
  set_variable(centring_, param);
}
void stiX::screen_formatter::set_underline(command_parameter param) {
  set_variable(underline_, param);
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
size_t stiX::screen_formatter::lines_remaining() const {
  return page_length_ - current_line_;
}


///////////
void stiX::format(std::istream& in, std::ostream& out) {
  auto formatter = screen_formatter { in, out };
  formatter.format();
}

void stiX::format(std::istream& in, std::ostream& out, size_t page_width, size_t page_length) {
  auto formatter = screen_formatter { in, out, page_width, page_length };
  formatter.format();
}