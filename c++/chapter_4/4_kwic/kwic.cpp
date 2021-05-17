#include "kwic.hpp"
#include "../../lib/join.hpp"
#include "../../lib/getline.hpp"
#include <sstream>
#include <iterator>

static std::vector<std::string> generate_rotations(std::string const &line, char fold_marker);
static std::vector<std::string> split_into_words(std::string const& input);
static std::string make_rotated_line(std::vector<std::string> const& words, size_t fold_word, char fold_marker);

auto const eof = std::char_traits<char>::eof();

void stiX::kwic(std::istream& in, std::ostream& out, char fold_marker) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    auto rotations = generate_rotations(line, fold_marker);
    for (auto& rotation: rotations)
      out << rotation << "\n";
  };
}

std::vector<std::string> generate_rotations(const std::string &line, char fold_marker) {
  if (line.empty())
    return { };

  auto words = split_into_words(line);

  auto rotations = std::vector<std::string> { };

  size_t fold_word = words.size() - 1;
  for (size_t rotation = 0; rotation != words.size(); ++rotation, --fold_word) {
    rotations.emplace_back(make_rotated_line(words, fold_word, fold_marker));
    std::rotate(words.begin(), words.begin()+1, words.end());
  }

  return rotations;
}

std::vector<std::string> split_into_words(std::string const& input) {
  auto iss = std::istringstream { input };
  auto words = std::vector<std::string> {
    std::istream_iterator<std::string>{iss},
    std::istream_iterator<std::string>()
  };
  return words;
}

std::string make_rotated_line(std::vector<std::string> const& words, size_t fold_word, char fold_marker) {
  auto out = std::ostringstream { };

  for (size_t i = 0; i != words.size(); ++i) {
    out << words[i];
    out << ((i == fold_word) ? fold_marker : ' ');
  }

  return out.str();
}


