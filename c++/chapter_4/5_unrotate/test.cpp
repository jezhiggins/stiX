#include "../../testlib/testlib.hpp"
#include "unrotate.hpp"

void testUnrotateLine(std::string const& input, std::string const& expected);

TEST_CASE("Chapter 4 - unrotate") {
  SECTION("empty input") {
    testUnrotateLine("", "");
  }

  SECTION("marker at end") {
    testUnrotateLine(
      "This is a test.$",
      "                 This is a test."
    );
  }
  SECTION("marker after first word") {
    testUnrotateLine(
      "test.$This is a",
      "      This is a  test."
    );
  }
  SECTION("marker in middle") {
    testUnrotateLine(
      "a test.$This is",
      "        This is  a test."
    );
  }
}

void testUnrotateLine(std::string const& input, std::string const& expected) {
  auto result = stiX::unrotateLine(input, 32, '$');
  REQUIRE(result == expected);
}
