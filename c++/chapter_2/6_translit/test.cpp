#include "../../testlib/testlib.h"

#include "translit.hpp"
#include "translit-args.hpp"

void testExpandCharacterClasses(
    std::string arg,
    std::string expected
);

void testExpandSrcCharacterClasses(
    std::string arg,
    std::string expected
);

void testTranslit(
    std::string replace, std::string with,
    std::string input, std::string expected
);

void testNegateTranslit(
    std::string replace, std::string with,
    std::string input, std::string expected
);

TEST_CASE("Chapter 2 - translit - argument expansion") {
    SECTION("is negated argument") {
        SECTION("'normal' is not") {
            REQUIRE(stiX::isNegatedArgument("normal") == false);
        }
        SECTION("'^negate' is") {
            REQUIRE(stiX::isNegatedArgument("^negated") == true);
        }
    }
    SECTION("good argument expansions") {
        testExpandCharacterClasses("", "");
        testExpandCharacterClasses("abcdef", "abcdef");
        testExpandCharacterClasses("a-d", "abcd");
        testExpandCharacterClasses("a-dA-D", "abcdABCD");
        testExpandCharacterClasses("0-9", "0123456789");
        testExpandCharacterClasses("a-c-e", "abcde");
        testExpandCharacterClasses("^abcdef", "^abcdef");
        testExpandCharacterClasses("^a-d", "^abcd");
        testExpandCharacterClasses("^a-dA-D", "^abcdABCD");
        testExpandCharacterClasses("^0-9", "^0123456789");
        testExpandCharacterClasses("^a-c-e", "^abcde");
        testExpandSrcCharacterClasses("abcdef", "abcdef");
        testExpandSrcCharacterClasses("a-d", "abcd");
        testExpandSrcCharacterClasses("a-dA-D", "abcdABCD");
        testExpandSrcCharacterClasses("0-9", "0123456789");
        testExpandSrcCharacterClasses("a-c-e", "abcde");
        testExpandSrcCharacterClasses("^abcdef", "abcdef");
        testExpandSrcCharacterClasses("^a-d", "abcd");
        testExpandSrcCharacterClasses("^a-dA-D", "abcdABCD");
        testExpandSrcCharacterClasses("^0-9", "0123456789");
        testExpandSrcCharacterClasses("^a-c-e", "abcde");
        testExpandSrcCharacterClasses("", "");
        testExpandSrcCharacterClasses("^", "");
    }
    SECTION("malformed") {
        testExpandCharacterClasses("a-0", "a-0");
        testExpandCharacterClasses("9-0", "9-0");
        testExpandCharacterClasses("0-a", "0-a");
        testExpandCharacterClasses("z-a", "z-a");
        testExpandCharacterClasses("a-Z", "a-Z");
        testExpandCharacterClasses("A-z", "A-z");
        testExpandCharacterClasses("---", "---");
        testExpandCharacterClasses("-z", "-z");
        testExpandCharacterClasses("a-", "a-");
        testExpandCharacterClasses("{-}", "{-}");
    }
    SECTION("escape sequence") {
        testExpandCharacterClasses("@", "@");
        testExpandCharacterClasses("@a", "a");
        testExpandCharacterClasses("@n", "\n");
        testExpandCharacterClasses("@t", "\t");
        testExpandCharacterClasses("@n@t", "\n\t");
        testExpandCharacterClasses("@t@n", "\t\n");
        testExpandCharacterClasses("@@", "@");
        testExpandCharacterClasses("@@@t@", "@\t@");
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

TEST_CASE("Chapter 2 - translit - squashing") {
    SECTION("two onto one replacement") {
        testTranslit("xy", "X", "abc", "abc");
        testTranslit("xy", "X", "xyz", "Xz");
        testTranslit("yz", "Y", "xyz", "xY");
        testTranslit("xz", "X", "xyz", "XyX");
        testTranslit("xy", "X", "xxz", "Xz");
        testTranslit("xy", "X", "zxx", "zX");
        testTranslit("xy", "X", "xxx", "X");
        testTranslit("xy", "X", "axxxa", "aXa");
        testTranslit("xy", "X", "ayyya", "aXa");
        testTranslit("xy", "X", "axyxa", "aXa");
        testTranslit("xy", "X", "axaxa", "aXaXa");
    }
    SECTION("three onto two character replacement") {
        testTranslit("xyz", "XY", "abc", "abc");
        testTranslit("xyz", "XY", "xyz", "XY");
        testTranslit("yzx", "YZ", "xyz", "ZYZ");
        testTranslit("zxy", "ZX", "xyz", "XZ");
        testTranslit("xyz", "XY", "xxz", "XXY");
        testTranslit("xyz", "XY", "zxx", "YXX");
        testTranslit("xyz", "XY", "xxx", "XXX");
        testTranslit("xyz", "XY", "yyy", "Y");
        testTranslit("xyz", "XY", "zzz", "Y");
        testTranslit("xyz", "XY", "xxxyyyzzz", "XXXY");
        testTranslit("xyz", "XY", "yyyxxxzzz", "YXXXY");
        testTranslit("xyz", "XY", "zzzyyyxxx", "YXXX");
        testTranslit("xyz", "XY", "ayyya", "aYa");
        testTranslit("xyz", "XY", "ayaya", "aYaYa");
    }
}

TEST_CASE("Chapter 2 - translit - deletion") {
    SECTION("single character removal") {
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

    SECTION("two character removal") {
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

TEST_CASE("Chapter 2 - translit - negated deletion") {
    testNegateTranslit("xy", "", "abc", "");
    testNegateTranslit("xy", "", "xyz", "xy");
    testNegateTranslit("yz", "", "xyz", "yz");
    testNegateTranslit("xz", "", "xyz", "xz");
    testNegateTranslit("xy", "", "xxz", "xx");
    testNegateTranslit("xy", "", "zxx", "xx");
    testNegateTranslit("xy", "", "xxx", "xxx");
    testNegateTranslit("xy", "", "axxxa", "xxx");
    testNegateTranslit("xy", "", "axaxa", "xx");
}

TEST_CASE("Chapter 2 - translit - negated squashing") {
    testNegateTranslit("xy", "X", "abc", "X");
    testNegateTranslit("xy", "X", "xyz", "xyX");
    testNegateTranslit("yz", "X", "xyz", "Xyz");
    testNegateTranslit("xz", "X", "xyz", "xXz");
    testNegateTranslit("xy", "X", "xxz", "xxX");
    testNegateTranslit("xy", "X", "zxx", "Xxx");
    testNegateTranslit("xy", "X", "xxx", "xxx");
    testNegateTranslit("xy", "X", "axxxa", "XxxxX");
    testNegateTranslit("xy", "X", "axaxa", "XxXxX");
    testNegateTranslit("xy", "Y", "xxxyyyzzz", "xxxyyyY");
    testNegateTranslit("xz", "Y", "yyyxxxzzz", "Yxxxzzz");
    testNegateTranslit("yz", "Y", "zzzyyyxxx", "zzzyyyY");
    testNegateTranslit("xyz", "Y", "ayyya", "YyyyY");
    testNegateTranslit("xyz", "Y", "ayaya", "YyYyY");
}

void testExpandCharacterClasses(
    std::string arg,
    std::string expected
) {
  DYNAMIC_SECTION("expandDestArgument('" << arg << "') gives '" << escape(expected) << "'") {
    REQUIRE(stiX::translitDestArgument(arg) == expected);
  }
}

void testExpandSrcCharacterClasses(
    std::string arg,
    std::string expected
) {
  DYNAMIC_SECTION("expandSrcArgument('" << arg << "') gives '" << escape(expected) << "'") {
    REQUIRE(stiX::translitSrcArgument(arg) == expected);
  }
}

void exerciseTranslit(
    std::string replace, std::string with,
    stiX::Selection selectionMode,
    std::string input, std::string expected
) {
  auto mode = (selectionMode == stiX::Selection::Normal)
      ? "Normal"
      : "Negate";
  DYNAMIC_SECTION("translit(" << replace << ", " << with << ", " << mode << ") on '" << input << "' gives '" << expected << "'") {
    std::istringstream is(input);
    std::ostringstream os;

    stiX::translit(replace, with, selectionMode, is, os);

    REQUIRE(os.str() == expected);
  }
}

void testTranslit(
    std::string replace, std::string with,
    std::string input, std::string expected
) {
    exerciseTranslit(
        replace, with, stiX::Selection::Normal,
        input, expected
    );
}
void testNegateTranslit(
    std::string replace, std::string with,
    std::string input, std::string expected
) {
    exerciseTranslit(
        replace, with, stiX::Selection::Negate,
        input, expected
    );
}
