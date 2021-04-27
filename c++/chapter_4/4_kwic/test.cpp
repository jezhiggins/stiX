#include "../../testlib/testlib.h"
#include "kwic.hpp"

using lines_t = std::vector<std::string>;
void testKwicLine(std::string const& input, lines_t const& expected);
void testKwic(std::string const& input, std::string const& expected);

TEST_CASE("Chapter 4 - kwic") {
  SECTION("empty input line") {
    testKwicLine("", {});
  }
  SECTION("line of one word") {
    testKwicLine("hotpot", {"hotpot$"});
  }
  SECTION("four word line has four rotations") {
    testKwicLine(
      "This is a test.",
      {
        "This is a test.$",
        "is a test.$This ",
        "a test.$This is ",
        "test.$This is a "
      }
    );
  }
  SECTION("trim leading spaces from line") {
    testKwicLine(
      "              This is a test.",
      {
        "This is a test.$",
        "is a test.$This ",
        "a test.$This is ",
        "test.$This is a "
      }
    );
  }
  SECTION("trim trailing space from line") {
    testKwicLine(
      "This is a test.                 ",
      {
        "This is a test.$",
        "is a test.$This ",
        "a test.$This is ",
        "test.$This is a "
      }
    );
  }
  SECTION("empty input") {
    testKwic("", "");
  }
  SECTION("one line of input") {
    testKwic(
      "This is a test.",
      "This is a test.$\n"
      "is a test.$This \n"
      "a test.$This is \n"
      "test.$This is a \n"
    );
  }
  SECTION("multiples lines of input") {
    testKwic(
      "Mary had a little lamb,\n"
      "Its fleece was white",
      "Mary had a little lamb,$\n"
      "had a little lamb,$Mary \n"
      "a little lamb,$Mary had \n"
      "little lamb,$Mary had a \n"
      "lamb,$Mary had a little \n"
      "Its fleece was white$\n"
      "fleece was white$Its \n"
      "was white$Its fleece \n"
      "fleece was white$Its \n"
    );
  }
}

void testKwicLine(std::string const& input, lines_t const& expected) {
  auto rotations = stiX::kwic_line(input);
  REQUIRE(rotations.size() == expected.size());
  REQUIRE(rotations == expected);
}

void testKwic(std::string const& input, std::string const& expected) {
  auto in = std::istringstream { input };
  auto out = std::ostringstream { };

  stiX::kwic(in, out);

  REQUIRE(out.str() == expected);
}
