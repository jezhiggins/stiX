#ifndef STICPP_FORMAT_FUNCTIONS_HPP
#define STICPP_FORMAT_FUNCTIONS_HPP

#include <string>
#include <vector>

namespace stiX {
  std::string fill_line(std::string const& line_in, size_t width);
  std::string centre_line(std::string const& line_in, size_t width);
  std::string underline(std::string const& line_in);
  std::string embolden(std::string const& line_in);
  std::string italicise(std::string const& line_in);
  std::string strikethrough(std::string const& line_in);

  size_t width_of(std::string const& w);

  struct word_width {
    std::string const word;
    size_t const width;
  };
  std::vector<word_width> split_into_words(std::string const& line_in);
}

#endif //STICPP_FORMAT_FUNCTIONS_HPP
