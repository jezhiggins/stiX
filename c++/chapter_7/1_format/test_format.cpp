#include "../../testlib/testlib.hpp"

#include "format.hpp"

void format_test(std::string input, std::string expected);

TEST_CASE("unformatted text") {
  SECTION("short line") {
    format_test("hello\n", "hello\n");
  }

  SECTION("two short lines, one short output line") {
    format_test("hello\nworld\n", "hello world\n");
  }
}

void format_test(std::string input, std::string expected) {
  auto in = std::istringstream { };
  in.str(input);

  auto out = std::ostringstream { };
  auto formatter = stiX::formatter { };
  formatter(in, out);

  auto output = out.str();
  REQUIRE(output == expected);
}