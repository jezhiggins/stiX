#include "../../testlib/testlib.h"

#include "translit.hpp"

void testTranslit(
    std::string replace, std::string with,
    std::string input, std::string expected
);

TEST_CASE("Chapter 2 - translit - single character replacement") {
  testTranslit("x", "X", "abc", "abc");
  testTranslit("x", "X", "xyz", "Xyz");
  testTranslit("y", "Y", "xyz", "xYz");
  testTranslit("z", "Z", "xyz", "xyZ");
  testTranslit("x", "X", "xxz", "XXz");
  testTranslit("x", "X", "zxx", "zXX");
  testTranslit("x", "X", "xxx", "XXX");
  testTranslit("x", "X", "axxxa", "aXXXa");
  testTranslit("x", "X", "axaxa", "aXaXa");
}

void testTranslit(
    std::string replace, std::string with,
    std::string input, std::string expected
) {
  DYNAMIC_SECTION("translit(" << replace << ", " << with << ") on '" << input << "' gives '" << expected << "'") {
    std::istringstream is(input);
    std::ostringstream os;

    stiX::translit(replace, with, is, os);

    REQUIRE(os.str() == expected);
  }
}
