
#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "1_copy/copy.h"
#include "2_charcount/charcount.h"
#include "3_linecount/linecount.h"

#include <tuple>

void test(std::string label, void (*fn)(std::string));
void verifyCopyString(std::string input);
void verifyCharCount(std::string input);
void verifyLineCount(std::string input);

////////////////////////////////////////////////////
////////////////////////////////////////////////////
TEST_CASE("Chapter 1 - copy") {
    test("copy", verifyCopyString);
}
TEST_CASE("Chapter 1 - charcount") {
    test("charcount", verifyCharCount);
}
TEST_CASE("Chapter 1 - linecount") {
    test("linecount", verifyLineCount);
}

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

const std::vector<std::string> test_strings = {
        empty,
        one_character,
        one_word_left_justified,
        one_word_right_justified,
        one_word_centred,
        longer,
        with_terminal_line_break,
        with_leading_line_break,
        line_breaks_before_and_aft,
        longer_with_multiple_line_breaks
};

std::string escape(std::string s);

void test(std::string label, void (*fn)(std::string)) {
    for (auto s : test_strings) {
        DYNAMIC_SECTION(label << "(\"" << escape(s) << "\")") {
            fn(s);
        }
    }
}

////////////////////
std::vector<std::tuple<std::string, std::string>> sequences = {
        { "\n", "\\n" },
        { "\t", "\\t" }
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

/////////
void verifyCopyString(std::string input)
{
    std::istringstream is(input);
    std::ostringstream os;

    stiX::copy(is, os);

    REQUIRE(os.str() == input );
}

void verifyCharCount(std::string input)
{
    std::istringstream is(input);

    auto count = stiX::charcount(is);

    REQUIRE(count == input.size());
}

void verifyLineCount(std::string input)
{
    std::istringstream is(input);

    auto count = stiX::linecount(is);

    size_t lines = 0;
    for (auto c : input)
        lines += (c == '\n');

    REQUIRE(count == lines);
}