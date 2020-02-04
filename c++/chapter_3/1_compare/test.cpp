#include "../../testlib/testlib.h"

#include "compare.hpp"

void testCompare(
    std::string const& left,
    std::string const& right,
    std::string const& expectedOutput
);

std::string const mary_had_a_little_lamb_its_fleece_was_white =
R"c(Mary had a little lamb
Its fleece was white as snow
And everywhere that Mary went
The lamb was sure to go
)c";

std::string const mary_had_a_little_lamb_its_fleece_was_kind_of_creamy =
R"c(Mary had a little lamb
Its fleece was a sort of porridge colour
And everywhere that Mary went
The lamb was sure to go
)c";

std::string const mary_had_a_little_lamb =
R"c(Mary had a little lamb
Its fleece was white as snow
)c";

std::string const santa_claus_is_coming_to_town =
R"c(You better watch out
You better not cry
You better not pout
I'm telling you why
)c";

TEST_CASE("Chapter 3 - compare") {
    SECTION("identical") {
        testCompare(
            mary_had_a_little_lamb,
            mary_had_a_little_lamb,
            ""
        );
    }
    SECTION("left is prefix") {
        testCompare(
            mary_had_a_little_lamb,
            mary_had_a_little_lamb_its_fleece_was_white,
            "end of file on left\n"
        );
    }
    SECTION("right is prefix") {
        testCompare(
            mary_had_a_little_lamb_its_fleece_was_white,
            mary_had_a_little_lamb,
            "end of file on right\n"
        );
    }
    SECTION("contents are slightly different") {
        testCompare(
            mary_had_a_little_lamb_its_fleece_was_white,
            mary_had_a_little_lamb_its_fleece_was_kind_of_creamy,
        R"c(2:
Its fleece was white as snow
Its fleece was a sort of porridge colour
)c"
        );
    }
    SECTION("contents are entirely different") {
        testCompare(
            mary_had_a_little_lamb_its_fleece_was_white,
            santa_claus_is_coming_to_town,
        R"c(1:
Mary had a little lamb
You better watch out
2:
Its fleece was white as snow
You better not cry
3:
And everywhere that Mary went
You better not pout
4:
The lamb was sure to go
I'm telling you why
)c"
        );
    }
}

void testCompare(
    std::string const& left,
    std::string const& right,
    std::string const& expectedOutput
) {
    std::istringstream lefts(left);
    std::istringstream rights(right);
    std::ostringstream output;

    stiX::compare(
        "left",
        lefts,
        "right",
        rights,
        output
    );
    REQUIRE(output.str() == expectedOutput);
} // testCompare

