
#include "../../testlib/testlib.h"

#include "overstrike.h"

void testOverstrike(std::string input, std::string expected);

TEST_CASE("Chapter 2 - overstrike") {
    testOverstrike("", "");
    testOverstrike("1", " 1");
    testOverstrike("\n", " \n");
    testOverstrike("Hello", " Hello");
    testOverstrike("Hello", " Hello");
    testOverstrike(
            "Hello\nWorld\n",
            " Hello\n World\n");

    testOverstrike(
            "Hello\b\b\b\b\b_____",
            " Hello\n+_____");
    testOverstrike(
            "Hello\b\b\b\b\b_____\nWorld\b\b\b\b\b_____",
            " Hello\n+_____\n World\n+_____");
    testOverstrike(
            "     Hello\b\b\b\b\b_____",
            "      Hello\n+     _____");

    testOverstrike("\b", "");
    testOverstrike("\b1", "\n+1");
    testOverstrike("1\b", " 1");
    testOverstrike(
            "Hello\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b_____",
            " Hello\n+_____");
    testOverstrike("\b\n", "\n+\n");
    testOverstrike(
            "\b\b\b\b\bHello=World\b\b\b\b\b_____",
            "\n+Hello=World\n+      _____");
    testOverstrike(
        "Hello\b\b\b\b\b_____=World\b\b\b\b\b_____",
        " Hello\n+_____=World\n+      _____");

}

void testOverstrike(std::string input, std::string expected)
{
    testFilter("overstrike", input, expected, stiX::overstrike);
}

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
