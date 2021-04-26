#include "../../testlib/testlib.h"
#include "kwic.h"

using lines_t = std::vector<std::string>;
void testKwic(std::string const& input, lines_t const& expected);

TEST_CASE("Chapter 4 - kwic") {
  SECTION("four word line has four rotations") {
    testKwic(
      "This is a test.",
      {
        "This is a test.$",
        "is a test.$This",
        "a test.$This is",
        "test.$This is a"
      }
    );
  }
}

void testKwic(std::string const& input, lines_t const& expected) {
  auto rotations = kwic(input);
  REQUIRE(rotations == expected);
}
