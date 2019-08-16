
#include "../../testlib/testlib.h"
#include "linecount.h"

void verifyLineCount(std::string input);

TEST_CASE("Chapter 1 - linecount") {
    verify_with_samples("linecount", verifyLineCount);
}

void verifyLineCount(std::string input)
{
    size_t lines = 0;
    for (auto c : input)
        lines += (c == '\n');

    std::istringstream is(input);
    auto count = stiX::linecount(is);

    REQUIRE(count == lines);
}
