//
// change  change patterns in text
//
// change pattern [newstuff]
//
// change copies its input to its output except that each
// non-overlapping string that matches pattern is replaced
// by newstuff. A non-existent newstuff implies deletion
// of the matched string. The patterns accept by change are
// the same as those used by find.
// The replacement string newstuff consists of zero or more
// of the following elements
//   c        literal character c
//   &        ditto, i.e. whatever was matched
//   @c       escaped character c (e.g. @&)
//
// To parenthesize all sums and differences of identifiers
//   change "*[a-zA-Z][a-zA-Z0-9]*[ ]*[+-][ ]*[a-zA-Z][a-zA-Z0-9]*" (&)
//

#include "../../lib/arguments.hpp"
#include "../../lib/regex/change.hpp"
#include <iostream>

int main(int argc, char const* argv[]) {
  auto arguments = stiX::make_arguments(argc, argv);
  if (arguments.size() < 1 || arguments.size() > 2) {
    std::cout << argv[0] << " pattern [replacement]\n";
    return 0;
  }

  auto& pattern = arguments.front();
  auto replacement = arguments.size() == 2 ? arguments.back() : "";

  stiX::change(std::cin, std::cout, pattern, replacement);
}
