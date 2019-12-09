#include "../../testlib/testlib.h"

#include "include.hpp"
#include <sstream>

void testInclude(
    std::string input,
    std::string expectedOutput
);

const std::string mary_had_a_little_lamb =
R"c(Mary had a little lamb
Its fleece was white as snow
And everywhere that Mary went
The lamb was sure to go
)c";

TEST_CASE("Chapter 3 - include") {
    SECTION("no include directive") {
        testInclude(
            mary_had_a_little_lamb,
            mary_had_a_little_lamb
        );
    }
}

std::istringstream testIncludeOpener(std::string openname) {
    return std::istringstream("what");
}

void testInclude(
    std::string input,
    std::string expected
) {
    std::istringstream inputs(input);
    std::ostringstream output;

    stiX::expand_include(
        inputs,
        output,
        testIncludeOpener
    );
    REQUIRE(output.str() == expected);
}