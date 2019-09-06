#include "../../testlib/testlib.h"

#include "echo.h"

char const* argv[] = {
    "apple",
    "banana",
    "plum"
};

void testEcho(int n, std::string expected);

TEST_CASE("Chapter 2 - echo") {
    testEcho(0, "");
    testEcho(1, "apple");
}

void testEcho(int n, std::string expected) {
  DYNAMIC_SECTION("echo(" << n << ")") {
    std::ostringstream os;

    stiX::echo(n, argv, os);

    REQUIRE(os.str() == expected);
  }
}