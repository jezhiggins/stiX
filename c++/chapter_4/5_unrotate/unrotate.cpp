#include "unrotate.hpp"
#include "../../lib/getline.hpp"

#include <iostream>

static std::pair<std::string, size_t>
wrap_long_line(std::string const& after_fold, size_t before_fold_length, size_t half_length);

auto const eof = std::char_traits<char>::eof();

void stiX::unrotate(std::istream &in, std::ostream &out, size_t line_length, char fold_marker) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);
    out << unrotateLine(line, line_length, fold_marker) << '\n';
  };
}

std::string stiX::unrotateLine(std::string const& line, size_t line_length, char fold_marker) {
  auto fold_position = line.find(fold_marker);
  if (fold_position == std::string::npos)
    return line;

  auto half_length = (line_length / 2) - 1;

  auto after_fold = line.substr(fold_position + 1);
  auto before_fold = line.substr(0, fold_position);

  auto [wrapped, wrap_at] = wrap_long_line(after_fold, before_fold.length(), half_length);
  if (wrap_at)
    after_fold = after_fold.substr(wrap_at);
  auto lead_padding = std::string(half_length - after_fold.length(), ' ');

  auto output =
    lead_padding +
    after_fold +
    "  " +
    before_fold +
    wrapped;

  return output.substr(0, line_length);
}

std::pair<std::string, size_t>
wrap_long_line(std::string const& after_fold, size_t before_fold_length, size_t half_length) {
  if (after_fold.length() <= half_length)
    return std::pair<std::string, size_t>();

  auto wrap_at = after_fold.length() - half_length;
  auto wrapped = after_fold.substr(0, wrap_at);
  auto taken = before_fold_length + wrap_at;

  if (taken < half_length)
    wrapped.insert(0, half_length - taken, ' ');
  else
    wrapped.insert(0, 1, ' ');

  return std::make_pair(wrapped, wrap_at);
}

