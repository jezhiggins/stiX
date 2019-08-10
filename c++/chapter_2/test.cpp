
#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "1_entab/entab.h"
#include "../lib/tab_stops.h"
#include "2_overstrike/overstrike.h"

void testEntab(std::string input, std::string expected);
void testOverstrike(std::string input, std::string expected);
void testFilter(
        std::string label,
        std::string input,
        std::string expected,
        void (*fn)(std::istream&, std::ostream&)
);

////////////////////////////////////////////////////
////////////////////////////////////////////////////
const std::string empty;
const std::string one_character("1");
const std::string longer("Hello World");

TEST_CASE("Chapter 2 - entab - is_tab_stop") {
    for (size_t p = 1; p != stiX::tabSize; ++p) {
        REQUIRE(!stiX::is_tab_stop(p));
        REQUIRE(stiX::is_tab_stop(p * stiX::tabSize));
    }
}

TEST_CASE("Chapter 2 - entab") {
    testEntab(empty, empty);
    testEntab(one_character, one_character);
    testEntab(longer,longer);
    testEntab("    Hello", "\tHello");
    testEntab("Hello   World", "Hello\tWorld");
    testEntab("1234567 ===", "1234567\t===");
    testEntab("12345678    ===", "12345678\t===");
    testEntab("    Hello", "\tHello");
    testEntab("     Hello", "\t Hello");
    testEntab("      Hello", "\t  Hello");
    testEntab("       Hello", "\t   Hello");
    testEntab("        Hello", "\t\tHello");

    testEntab("\n    Hello", "\n\tHello");
    testEntab("\n    \nHello", "\n\t\nHello");
    testEntab("\n\n    Hello", "\n\n\tHello");
    testEntab("Hello\n    World", "Hello\n\tWorld");
}

void testEntab(std::string input, std::string expected)
{
    testFilter("entab", input, expected, stiX::entab);
}

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

std::string escape(std::string s);

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

////////////////////
std::vector<std::tuple<std::string, std::string>> sequences = {
        { "\n", "\\n" },
        { "\t", "\\t" },
        { "\b", "\\b" }
};
std::string escape(std::string s) {
    for (auto esc : sequences ) {
        auto f = std::get<0>(esc);
        auto r = std::get<1>(esc);

        for(size_t pos = s.find(f); pos != std::string::npos; pos = s.find(f))
            s.replace(pos, f.size(), r);

    }
    return s;
}

