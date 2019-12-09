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

const std::string line_without_newline =
    "Fruit!";
const std::string line_with_newline =
    "Fruit!\n";

const std::string include_mary =
    "#include \"mary\"";

TEST_CASE("Chapter 3 - include") {
    SECTION("no include directive") {
        for (auto l : { line_with_newline, line_without_newline, mary_had_a_little_lamb })
            testInclude(
                l,
                l
            );
    }

    SECTION("include directive") {
        testInclude(
            include_mary,
            mary_had_a_little_lamb
        );
    }

} // TEST_CASE

std::istringstream testIncludeOpener(std::string openname) {
    std::string output = "error";
    if (openname == "mary")
        output = mary_had_a_little_lamb;
    return std::istringstream(mary_had_a_little_lamb);
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