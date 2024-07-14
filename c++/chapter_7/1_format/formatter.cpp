#include "formatter.hpp"
#include "format.hpp"

#include <sstream>
#include <string>
#include <charconv>
#include <functional>
#include "format_functions.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include "../../lib/regex/replacement.hpp"
#include "../../lib/regex/change.hpp"

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
  centering_(0),
  underline_(0),
  bold_(0),
  fill_(true),
  indent_(0),
  next_indent_(),
  current_line_(0),
  current_page_(1),
  output_mode_(&screen_formatter::buffer_line) {
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
  auto title = line.substr(line.find_first_not_of(blank, 3));
  auto leading_quote = !title.empty() &&
    (title[0] == '\'' || title[0] == '\"');
  if (leading_quote)
    title = title.substr(1);
  return title;
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
  if (command == ".bo")
    set_bold(param(default_active_lines));
  if (command == ".in")
    set_indent(param(0));
  if (command == ".ti")
    set_next_indent(param(0));
  if (command == ".he")
    set_header(string_param(line));
  if (command == ".fo")
    set_footer(string_param(line));
  if (command == ".bp")
    page_break(param(current_page_+1));
}

void stiX::screen_formatter::handle_text(std::string line) {
  leading_blanks(line);

  apply_styles(line);

  if (line.empty()) {
    print_blank_line();
    return;
  }

  output_line(line);
}

void stiX::screen_formatter::output_line(std::string const& line) {
  std::invoke(output_mode_, this, line);
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

void stiX::screen_formatter::apply_styles(std::string& line) {
  apply_underlining(line);
  apply_bold(line);
}

void stiX::screen_formatter::apply_underlining(std::string& line) {
  if (!underline_)
    return;

  --underline_;
  line = stiX::underline(line);
}
void stiX::screen_formatter::apply_bold(std::string& line) {
  if (!bold_)
    return;

  --bold_;
  line = stiX::embolden(line);
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
  print_line(centre_line(line, fillable_width()));

  if (--centering_ == 0)
    set_output_mode();
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

  if (current_line_ == bottom_margin()) {
    print_footer();

    current_line_ = 0;
    ++current_page_;
  }
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
void stiX::screen_formatter::put_line(std::string const& line) {
  out_ << line;
  line_feed();
}
void stiX::screen_formatter::line_feed() {
  out_ << '\n';
  ++current_line_;
}
void stiX::screen_formatter::line_feed(size_t count) {
  for (auto c = 0; c != count; ++c)
    line_feed();
}

void stiX::screen_formatter::page_end() {
  flush();

  while (current_line_ != 0)
    print_blank_line();
}

void stiX::screen_formatter::print_header() {
  print_title(header_, hf_margin_above, hf_margin_below);
}
void stiX::screen_formatter::print_footer() {
  print_title(footer_, hf_margin_below, hf_margin_above);
}
void stiX::screen_formatter::print_title(
    std::string const& title,
    size_t margin_before,
    size_t margin_after) {
  if (title.empty())
    return;

  auto matcher = compile_pattern("#");
  auto replacer = prepare_replacement(std::to_string(current_page_));
  auto o = std::ostringstream { };
  apply_change(matcher, replacer, title, o);

  line_feed(margin_before);
  put_line(o.str());
  line_feed(margin_after);
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
  set_output_mode();
}

void stiX::screen_formatter::set_output_mode() {
  output_mode_ = fill_
    ? &screen_formatter::buffer_line
    : &screen_formatter::print_line;
}

void stiX::screen_formatter::vertical_space(command_parameter param) {
  flush();

  auto lines = size_t{0};
  set_variable(lines, param, 0, lines_remaining());

  for (auto l = 0; l != lines; ++l)
    print_blank_line();
}
void stiX::screen_formatter::page_break(command_parameter param) {
  page_end();
  set_variable(current_page_, param);
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
  set_variable(centering_, param);
  if (centering_)
    output_mode_ = &screen_formatter::print_centred_line;
  else
    set_output_mode();
}
void stiX::screen_formatter::set_underline(command_parameter param) {
  set_variable(underline_, param);
}
void stiX::screen_formatter::set_bold(command_parameter param) {
  set_variable(bold_, param);
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
void stiX::screen_formatter::set_footer(std::string const& footer) {
  footer_ = footer;
}

size_t stiX::screen_formatter::fillable_width() const {
  return right_margin_ - indent();
}
size_t stiX::screen_formatter::indent() const {
  return next_indent_.value_or(indent_);
}
size_t stiX::screen_formatter::lines_remaining() const {
  return bottom_margin() - current_line_;
}
size_t stiX::screen_formatter::bottom_margin() const {
  auto footer_length = footer_.empty()
    ? 0
    : hf_margin_above + hf_margin_below + 1;
  return page_length_ - footer_length;
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