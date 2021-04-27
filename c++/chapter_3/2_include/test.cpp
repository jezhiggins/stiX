#include "../../testlib/testlib.hpp"

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

const std::string include_include_mary =
    "#include \"include_mary\"";

const std::string bad_include =
R"c(Here is a line
#include "missing"
And another line
)c";

const std::string expanded_bad_include =
R"c(Here is a line
ERROR: Could not include "missing"
)c";


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

    SECTION("nested-include directives") {
        testInclude(
            include_include_mary,
            mary_had_a_little_lamb
        );
    }

    SECTION("include non-existant file") {
        testInclude(
            bad_include,
            expanded_bad_include
        );
    }

} // TEST_CASE

std::istringstream testIncludeOpener(std::string const& openname) {
    std::string output;
    if (openname == "mary")
        output = mary_had_a_little_lamb;
    else if (openname == "include_mary")
        output = include_mary;
    auto ss = std::istringstream(output);
    if (output.empty())
        ss.setstate(std::ios_base::badbit);
    return ss;
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