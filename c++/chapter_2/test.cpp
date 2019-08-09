
#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "1_entab/entab.h"
#include "../lib/tab_stops.h"
#include "2_overstrike/overstrike.h"

void testEntab(std::string input, std::string expected);
void testOverstrike(std::string input, std::string expected);
void testFilter(
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
    testFilter(input, expected, stiX::entab);
}

TEST_CASE("Chapter 2 - overstrike") {
    testOverstrike("Hello", " Hello");
    testOverstrike("Hello\b\b\b\b\b_____", " Hello\n+_____");
}

void testOverstrike(std::string input, std::string expected)
{
    testFilter(input, expected, stiX::overstrike);
}

void testFilter(
        std::string input,
        std::string expected,
        void (*fn)(std::istream&, std::ostream&)
) {
    std::istringstream is(input);
    std::ostringstream os;

    fn(is, os);

    REQUIRE(os.str() == expected);
}
