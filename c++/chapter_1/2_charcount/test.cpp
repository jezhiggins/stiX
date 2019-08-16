
#include "../../testlib/testlib.h"
#include "charcount.h"

void verifyCharCount(std::string input);
TEST_CASE("Chapter 1 - charcount") {
    verify_with_samples("charcount", verifyCharCount);
}

void verifyCharCount(std::string input)
{
    std::istringstream is(input);

    auto count = stiX::charcount(is);

    REQUIRE(count == input.size());
}
