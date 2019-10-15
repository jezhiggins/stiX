#include "../../testlib/testlib.h"

#include "translit.hpp"
#include "translit-args.hpp"

void testExpandArg(
    std::string arg,
    std::string expected
);

void testTranslit(
    std::string replace, std::string with,
    std::string input, std::string expected
);

TEST_CASE("Chapter 2 - translit - good argument expansions") {
  testExpandArg("abcdef", "abcdef");
  testExpandArg("a-d", "abcd");
  testExpandArg("a-dA-D", "abcdABCD");
  testExpandArg("0-9", "0123456789");
  testExpandArg("a-c-e", "abcde");
}
TEST_CASE("Chapter 2 - translit - malformed argument expansions") {
  testExpandArg("a-0", "a-0");
  testExpandArg("9-0", "9-0");
  testExpandArg("0-a", "0-a");
  testExpandArg("z-a", "z-a");
  testExpandArg("a-Z", "a-Z");
  testExpandArg("A-z", "A-z");
  testExpandArg("---", "---");
  testExpandArg("-z", "-z");
  testExpandArg("a-", "a-");
  testExpandArg("{-}", "{-}");
}
TEST_CASE("Chapter 2 - translit - escape sequence expansion") {
  testExpandArg("@", "@");
  testExpandArg("@a", "a");
  testExpandArg("@n", "\n");
  testExpandArg("@t", "\t");
  testExpandArg("@n@t", "\n\t");
  testExpandArg("@t@n", "\t\n");
  testExpandArg("@@", "@");
  testExpandArg("@@@t@", "@\t@");
}

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

TEST_CASE("Chapter 2 - translit - two character replacement") {
  testTranslit("xy", "XY", "abc", "abc");
  testTranslit("xy", "XY", "xyz", "XYz");
  testTranslit("yz", "YZ", "xyz", "xYZ");
  testTranslit("xz", "XZ", "xyz", "XyZ");
  testTranslit("xy", "XY", "xxz", "XXz");
  testTranslit("xy", "XY", "zxx", "zXX");
  testTranslit("xy", "XY", "xxx", "XXX");
  testTranslit("xy", "XY", "axxxa", "aXXXa");
  testTranslit("xy", "XY", "axaxa", "aXaXa");
}

void testExpandArg(
    std::string arg,
    std::string expected
) {
  DYNAMIC_SECTION("expandArgument('" << arg << "') gives '" << escape(expected) << "'") {
    REQUIRE(stiX::translitArgument(arg) == expected);
  }
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
