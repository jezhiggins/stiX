#include "test_filter.h"
#include "label_escape.h"
#include "catch.hpp"

#include <sstream>

void testFilter(
    std::string label,
    std::string input,
    std::string expected,
    void (*fn)(std::istream&, std::ostream&)
) {
  DYNAMIC_SECTION(label << "(\"" << escape(input) << "\")") {
    std::istringstream is(input);
    std::ostringstream os;

    fn(is, os);

    REQUIRE(os.str() == expected);
  }
}

