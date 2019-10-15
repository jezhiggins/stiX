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

TEST_CASE("Chapter 2 - translit - argument expansion") {
    SECTION("good argument expansions") {
        testExpandArg("abcdef", "abcdef");
        testExpandArg("a-d", "abcd");
        testExpandArg("a-dA-D", "abcdABCD");
        testExpandArg("0-9", "0123456789");
        testExpandArg("a-c-e", "abcde");
    }
    SECTION("malformed") {
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
    SECTION("escape sequence") {
        testExpandArg("@", "@");
        testExpandArg("@a", "a");
        testExpandArg("@n", "\n");
        testExpandArg("@t", "\t");
        testExpandArg("@n@t", "\n\t");
        testExpandArg("@t@n", "\t\n");
        testExpandArg("@@", "@");
        testExpandArg("@@@t@", "@\t@");
    }
}

TEST_CASE("Chapter 2 - translit - replacement") {
    SECTION("single character replacement") {
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

    SECTION("two character replacement") {
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
}

TEST_CASE("Chapter 2 - translit - deletion") {
    SECTION("single character replacement") {
        testTranslit("x", "", "abc", "abc");
        testTranslit("x", "", "xyz", "yz");
        testTranslit("y", "", "xyz", "xz");
        testTranslit("z", "", "xyz", "xy");
        testTranslit("x", "", "xxz", "z");
        testTranslit("x", "", "zxx", "z");
        testTranslit("x", "", "xxx", "");
        testTranslit("x", "", "axxxa", "aa");
        testTranslit("x", "", "axaxa", "aaa");
    }

    SECTION("two character replacement") {
        testTranslit("xy", "", "abc", "abc");
        testTranslit("xy", "", "xyz", "z");
        testTranslit("yz", "", "xyz", "x");
        testTranslit("xz", "", "xyz", "y");
        testTranslit("xy", "", "xxz", "z");
        testTranslit("xy", "", "zxx", "z");
        testTranslit("xy", "", "xxx", "");
        testTranslit("xy", "", "axxxa", "aa");
        testTranslit("xy", "", "axaxa", "aaa");
    }
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
