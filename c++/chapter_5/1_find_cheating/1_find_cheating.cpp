//
// find   find patterns in text
//
// find pattern
//
// find treads its input a line at a time and writes to its output
// those lines that match the specified text pattern. A text
// pattern is a concatenation of the following elements
//   c        literal character c
//   ?        any character except newline
//   %        beginning of the line
//   $        end of the line (null string before newline)
//   [...]    character class (any of these characters)
//   [^...]   negated character class (all but these characters)
//   *        closure (zero or more occurrences of previous pattern)
//   @c       escaped character (eg @%, @[, @*)
// Special meaning of characters in a text pattern is lost when
// escaped, inside [...] (except @]), or for:
//   %        not at beginning
//   $        not at end
//   *        at beginning
// A character class consists of zero of more of the following
// elements, surrounded by [ and ]:
//   c        literal character c, including [
//   c1-c2    range of characters (digits, lower or upper case letters)
//   ^        negated character class if at beginning
//   @c       escaped character (eg @^ @- @@ @])
// Special meaning of characters in a character class is lost when
// escaped or for:
//   ^        not at beginning
//   -        at beginning or end
// An escape sequence consists of the character @ followed by a
// single character:
//   @n       newline
//   @t       tab
//   @c       c (including @@)
//
// To print line ending in a Pascal keyword or identifier:
//   find [a-zA-Z][a-zA-Z0-9]*$
//

#include "../../lib/arguments.hpp"
#include "../../lib/getline.hpp"
#include <regex>
#include <iostream>

std::string translate_pattern(std::string const& pattern);

int main(int argc, char const* argv[]) {
  auto arguments = stiX::make_arguments(argc, argv);
  if (arguments.size() != 1) {
    std::cout << argv[0] << " pattern" << std::endl;
    return 0;
  }

  auto modern_pattern = translate_pattern(arguments.front());
  auto matcher = std::regex { modern_pattern,  std::regex_constants::ECMAScript };

  while(std::cin) {
    auto line = stiX::getline(std::cin);

    if (std::regex_search(line, matcher))
      std::cout << line << '\n';
  }
}

std::string translate_pattern(std::string const& pattern) {
  auto fixedUpPattern = std::string { };
  auto escaped = false;
  auto classStart = false;

  for (size_t i = 0; i != pattern.size(); ++i) {
    char c = pattern[i];

    if (c == '%') {
      fixedUpPattern += (i != 0) ? c : '^';
    } else if (c == '@' && !escaped) {
      escaped = true;
      fixedUpPattern += '\\';
    } else if (c == '?' && !escaped) {
      fixedUpPattern += '.';
    } else if (c == '\\') {
      fixedUpPattern += "\\\\";
    } else if (c == '.') {
      fixedUpPattern += "\\.";
    } else if (c == '^' && !classStart) {
      fixedUpPattern += "\\^";
    } else {
      classStart = (c == '[');
      escaped = false;
      fixedUpPattern += c;
    }
  }

  return fixedUpPattern;
}
