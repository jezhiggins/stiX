
#include "../../testlib/testlib.h"
#include "../../lib/copy.hpp"

void verifyCopyString(std::string input);

TEST_CASE("Chapter 1 - copy") {
    verify_with_samples("copy", verifyCopyString);
}

void verifyCopyString(std::string input)
{
    std::istringstream is(input);
    std::ostringstream os;

    stiX::copy(is, os);

    REQUIRE(os.str() == input );
}
