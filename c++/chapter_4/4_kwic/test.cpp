#include "../../testlib/testlib.h"
#include "kwic.hpp"

using lines_t = std::vector<std::string>;
void testKwic(std::string const& input, lines_t const& expected);

TEST_CASE("Chapter 4 - kwic") {
  SECTION("no input") {
    testKwic("", { });
  }
  SECTION("one word") {
    testKwic("hotpot", { "hotpot$"});
  }
  SECTION("four word line has four rotations") {
    testKwic(
      "This is a test.",
      {
        "This is a test.$",
        "is a test.$This ",
        "a test.$This is ",
        "test.$This is a "
      }
    );
  }
  SECTION("trim leading space") {
    testKwic(
      "              This is a test.",
      {
        "This is a test.$",
        "is a test.$This ",
        "a test.$This is ",
        "test.$This is a "
      }
    );
  }
  SECTION("trim trailing space") {
    testKwic(
      "This is a test.                 ",
      {
        "This is a test.$",
        "is a test.$This ",
        "a test.$This is ",
        "test.$This is a "
      }
    );
  }
}

void testKwic(std::string const& input, lines_t const& expected) {
  auto rotations = stiX::kwic_line(input);
  REQUIRE(rotations.size() == expected.size());
  REQUIRE(rotations == expected);
}
