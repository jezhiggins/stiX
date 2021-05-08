#include "unrotate.hpp"

static std::pair<std::string, std::string>
wrap_long_line(std::string const& after_fold, size_t before_fold_length, size_t half_length);

std::string stiX::unrotateLine(std::string const& line, size_t line_length, char fold_marker) {
  auto fold_position = line.find(fold_marker);
  if (fold_position == std::string::npos)
    return line;

  auto half_length = (line_length / 2) - 1;

  auto after_fold = line.substr(fold_position + 1);
  auto before_fold = line.substr(0, fold_position);

  auto [post_padding, wrapped] = wrap_long_line(after_fold, before_fold.length(), half_length);
  if (!wrapped.empty())
    after_fold = after_fold.substr(wrapped.length());
  auto lead_padding = std::string(half_length - after_fold.length(), ' ');

  auto output =
    lead_padding +
    after_fold +
    "  " +
    before_fold +
    post_padding +
    wrapped;

  return output.erase(output.find_last_not_of(' ') + 1);
}

std::pair<std::string, std::string>
wrap_long_line(std::string const& after_fold, size_t before_fold_length, size_t half_length) {
  auto post_padding = std::string();
  auto wrapped = std::string();

  if (after_fold.length() > half_length) {
    auto wrap_at = after_fold.length() - half_length;
    wrapped = after_fold.substr(0, wrap_at);
    auto padding = before_fold_length + wrap_at;
    post_padding = std::string(half_length - padding, ' ');
  }

  return std::make_pair(post_padding, wrapped);
}

