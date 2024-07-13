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
  constinit auto blank = ' ';

  constinit auto default_right_margin = 60;
  constinit auto default_page_length = 66;
  constinit auto default_vertical_space = 1;
  constinit auto default_line_space = 1;
  constinit auto default_active_lines = 1;
  constinit auto hf_margin_above = 1;
  constinit auto hf_margin_below = 2;

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
  right_margin_(page_width),
  page_length_(page_length),
  line_space_(default_line_space),
  centring_(0),
  underline_(0),
  fill_(true),
  indent_(0),
  next_indent_(),
  current_line_(0) {
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

std::string stiX::screen_formatter::string_param(
  std::string const& line) const
{
  return line.substr(line.find_first_not_of(blank, 3));
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
  if (command == ".in")
    set_indent(param(0));
  if (command == ".ti")
    set_next_indent(param(0));
  if (command == ".he")
    set_header(string_param(line));
}

void stiX::screen_formatter::handle_text(std::string line) {
  leading_blanks(line);

  apply_underlining(line);

  if (line.empty()) {
    print_blank_line();
    return;
  }

  if (centring_) {
    print_centred_line(line);
    return;
  }
  if (!fill_)
    print_line(line);
  else
    buffer_line(line);
}

void stiX::screen_formatter::leading_blanks(std::string &line) {
  if (line.empty() || line[0] != blank)
    return;

  auto last_leading_blank = line.find_first_not_of(blank);
  line = line.substr(last_leading_blank);

  if (!line.empty()) {
    flush();
    next_indent_ = last_leading_blank;
  }
}

void stiX::screen_formatter::apply_underlining(std::string &line) {
  if (!underline_)
    return;

  --underline_;
  line = stiX::underline(line);
}

void stiX::screen_formatter::buffer_line(std::string const& line) {
  for (auto word: split_into_words(line)) {
    if (count_width(buffer_) + word.width >= fillable_width())
      fill_and_flush();

    buffer_ += buffer_.empty() ? null : space;
    buffer_ += word.word;
  }
}

void stiX::screen_formatter::fill_and_flush() {
  buffer_ = fill_line(buffer_, fillable_width());
  flush();
}

void stiX::screen_formatter::flush() {
  if (buffer_.empty())
    return;

  print_line(buffer_);
  buffer_.clear();
}
void stiX::screen_formatter::print_centred_line(std::string const& line) {
  --centring_;
  print_line(centre_line(line, fillable_width()));
}
void stiX::screen_formatter::print_blank_line() {
  flush();
  print_line("");
}

void stiX::screen_formatter::print_line(std::string const& line) {
  if (current_line_ == 0)
    print_header();

  if (!line.empty())
    out_ << leading_indent() << line;

  line_spacing();
}
std::string stiX::screen_formatter::leading_indent() {
  auto i = indent();
  clear_next_indent();
  return std::string(i, blank);
}

void stiX::screen_formatter::line_spacing() {
  auto line_space = std::min(line_space_, lines_remaining());
  for (auto i = 0; i != line_space; ++i)
    line_feed();
}
void stiX::screen_formatter::line_feed() {
  out_ << '\n';
  if (++current_line_ == page_length_)
    current_line_ = 0;
}
void stiX::screen_formatter::line_feed(size_t count) {
  for (auto c = 0; c != count; ++c)
    line_feed();
}

void stiX::screen_formatter::page_end() {
  flush();

  while (current_line_ != 0)
    line_feed();
}

void stiX::screen_formatter::print_header() {
  if (header_.empty())
    return;

  line_feed(hf_margin_above);
  print_line(header_);
  line_feed(hf_margin_below);
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
void stiX::screen_formatter::set_indent(command_parameter param) {
  set_variable(indent_, param);
}
void stiX::screen_formatter::set_next_indent(command_parameter param) {
  auto next = indent_;
  set_variable(next, param);
  next_indent_ = next;
}
void stiX::screen_formatter::clear_next_indent() {
  next_indent_ = std::nullopt;
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

void stiX::screen_formatter::set_header(std::string const& header) {
  header_ = header;
}

size_t stiX::screen_formatter::fillable_width() const {
  return right_margin_ - indent();
}
size_t stiX::screen_formatter::indent() const {
  return next_indent_.value_or(indent_);
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