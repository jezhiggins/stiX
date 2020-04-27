
#include "../../testlib/testlib.h"

#include "detab.h"
#include "../../lib/tab_stops.hpp"

void testDetab(std::string input, std::string expected);

////////////////////////////////////////////////////
const std::string empty;
const std::string one_character("1");
const std::string longer("Hello World");

TEST_CASE("Chapter 1 - detab - next_tab_stop") {
    for (size_t p = 0; p != stiX::tabSize; ++p)
        REQUIRE(stiX::next_tab_stop(p) == stiX::tabSize);
    for (size_t p = stiX::tabSize; p != (stiX::tabSize * 2); ++p)
        REQUIRE(stiX::next_tab_stop(p) == (stiX::tabSize * 2));

    for (size_t s = 0; s <= 10; ++s) {
        const auto p = (s * stiX::tabSize) + (stiX::tabSize / 2);
        REQUIRE(stiX::next_tab_stop(p) == ((s+1) * stiX::tabSize));
    }
}

TEST_CASE("Chapter 1 - detab - distance_to_next_tab_stop") {
    for (size_t p = 0; p != stiX::tabSize; ++p) {
        auto remainder = stiX::tabSize - p;
        REQUIRE(stiX::distance_to_next_tab_stop(p) == remainder);
        REQUIRE(stiX::distance_to_next_tab_stop(p + stiX::tabSize) == remainder);
    }
}

TEST_CASE("Chapter 1 - detab") {
    testDetab(empty, empty);
    testDetab(one_character, one_character);
    testDetab(longer,longer);
    testDetab("\tHello", "    Hello");
    testDetab("Hello\tWorld", "Hello   World");
    testDetab("1234567\t===", "1234567 ===");
    testDetab("12345678\t===", "12345678    ===");
    testDetab("\t Hello", "     Hello");
    testDetab("\t  Hello", "      Hello");
    testDetab("\t   Hello", "       Hello");
    testDetab("\t    Hello", "        Hello");
    testDetab("\t\tHello", "        Hello");

    testDetab("\n\tHello", "\n    Hello");
    testDetab("\n\t\nHello", "\n    \nHello");
    testDetab("\n\n\tHello", "\n\n    Hello");
    testDetab("Hello\n\tWorld", "Hello\n    World");
}

void testDetab(std::string input, std::string expected) {
    testFilter("detab", input, expected, stiX::detab);
}