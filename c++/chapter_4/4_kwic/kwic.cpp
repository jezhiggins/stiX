#include "kwic.hpp"
#include "../../lib/join.hpp"
#include "../../lib/getline.hpp"
#include <sstream>
#include <iterator>

static std::vector<std::string> generate_rotations(std::string const &line);
static std::vector<std::string> split_into_words(std::string const& input);
static std::string make_rotated_line(std::vector<std::string> const& words);

auto const eof = std::char_traits<char>::eof();

void stiX::kwic(std::istream& in, std::ostream& out) {
  while(in.peek() != eof) {
    auto line = stiX::getline(in);

    auto rotations = generate_rotations(line);
    for (auto& rotation: rotations)
      out << rotation << "\n";
  };
}

std::vector<std::string> generate_rotations(const std::string &line) {
  if (line.empty())
    return { };

  auto words = split_into_words(line);

  auto rotations = std::vector<std::string> { };

  for (size_t rotation = 0; rotation != words.size(); ++rotation) {
    rotations.emplace_back(make_rotated_line(words));
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

  auto word = words.begin();
  auto const last_word = words.end() - 1;
  for (; word != last_word; ++word)
    *word += ' ';
  *word += '$';
  return words;
}

std::string make_rotated_line(std::vector<std::string> const& words) {
  auto out = std::ostringstream { };
  stiX::join(words, std::ostream_iterator<std::string>(out), "");
  return out.str();
}


