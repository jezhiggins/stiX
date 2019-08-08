
#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "1_entab/entab.h"
#include "../lib/tab_stops.h"

void test(std::string label, void (*fn)(std::string));
void testEntab(std::string input, std::string expected);

////////////////////////////////////////////////////
////////////////////////////////////////////////////
const std::string empty;
const std::string one_character("1");
const std::string one_word_left_justified("Hello     ");
const std::string one_word_right_justified("     Hello");
const std::string one_word_centred("  hello  ");
const std::string longer("Hello World");
const std::string with_terminal_line_break("The End\n");
const std::string with_leading_line_break("\nThe Beginning");
const std::string line_breaks_before_and_aft("\n  In the middle \n");
const std::string longer_with_multiple_line_breaks("\tHello\n\t\tWorld!\n");

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
    std::istringstream is(input);
    std::ostringstream os;

    stiX::entab(is, os);

    REQUIRE(os.str() == expected);
}