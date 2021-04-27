#include "../../testlib/testlib.hpp"
#include "kwic.hpp"

using lines_t = std::vector<std::string>;
void testKwic(std::string const& input, std::string const& expected);

TEST_CASE("Chapter 4 - kwic") {
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
      "white$Its fleece was \n"
    );
  }
  SECTION("trim leading spaces from line") {
    testKwic(
      "              This is a test.",
      "This is a test.$\n"
      "is a test.$This \n"
      "a test.$This is \n"
      "test.$This is a \n"
    );
  }
  SECTION("trim trailing space from line") {
    testKwic(
      "This is a test.                 ",
      "This is a test.$\n"
      "is a test.$This \n"
      "a test.$This is \n"
      "test.$This is a \n"
    );
  }
}

void testKwic(std::string const& input, std::string const& expected) {
  testFilter("kwic", input, expected,
 [](std::istream& in, std::ostream& out) {
      stiX::kwic(in, out, '$');
    }
  );
}
