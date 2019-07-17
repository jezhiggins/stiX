
#define CATCH_CONFIG_MAIN
#include "../catch.hpp"

#include "1_copy/copy.h"
#include "2_charcount/charcount.h"

#include <tuple>

void verifyCopyString(std::string input);
void verifyCharCount(std::string input);

const std::string empty;
const std::string very_short("1");
const std::string longer("Hello World");
const std::string longer_with_line_breaks("\tHello\n\t\tWorld!\n");

const std::vector<std::string> test_strings = {
        empty,
        very_short,
        longer,
        longer_with_line_breaks
};

std::string escape(std::string s);

//////////////////////////
TEST_CASE( "Chapter 1 - copy" ) {
    for (auto s : test_strings) {
        DYNAMIC_SECTION("copy(\"" << escape(s) << "\")") {
            verifyCopyString(s);
        }
    }
}

TEST_CASE( "Chapter 1 - charcount") {
    for (auto s : test_strings) {
        DYNAMIC_SECTION("charcount(\"" << escape(s) << "\")") {
            verifyCharCount(s);
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

    REQUIRE( os.str() == input );
}

void verifyCharCount(std::string input)
{
    std::istringstream is(input);

    auto count = stiX::charcount(is);

    REQUIRE(count == input.size());
}
