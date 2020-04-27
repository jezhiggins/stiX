//
// translit
//
// translit maps its input, on a character by character
// basis, and writes the translated version to its output.
// In the simplest case, each character in the argument src
// is translated to the corresponding character in the
// argument dest; all other characters are copied as is.
// Both src and dest may contain substrings of the form
// c1-c2 as shorthand for all of the characters in the range
// c1..c2. c1 and c2 must both be digits, or both be letters
// of the same case.
// If dest is absent, all characters represented by src are
// deleted. Otherwise if dest is shorter than src, all
// characters in src that would map to or beyond the last
// character in dest are mapped to the last character in
// dest; moreover adjacent instances of such characters in
// the input are represented in the output by a single
// instance of the last character in dest. Thus
//   translit 0-9 9
// converts each string of digits to the single digit 9
// Finally, if src is preceded by a ^, then all but the
// characters represented by src are taken as the source
// string; i.e. that are all deleted if dest is absent, or
// they are all collapsed if the last character in dest is
// present.

#include <iostream>
#include "../../lib/arguments.hpp"
#include "translit-args.hpp"
#include "translit.hpp"

int main(int argc, char const* argv[]) {
  auto arguments = stiX::make_arguments(argc, argv);
  if (arguments.size() < 1 || arguments.size() > 2) {
    std::cout << argv[0] << "[^]src [dest]" << std::endl;
    return 0;
  }
  arguments.emplace_back("");

  stiX::translit(
      stiX::translitSrcArgument(arguments[0]),
      stiX::translitDestArgument(arguments[1]),
      stiX::Selection(stiX::isNegatedArgument(arguments[0])),
      std::cin,
      std::cout
  );
}