#include "../../testlib/testlib.hpp"

#include "formatter.hpp"

struct test_options {
  bool const no_fill;
};

void format_test(
  std::string input,
  std::string expected,
  test_options opts = { });

constinit auto no_fill = test_options { .no_fill = true };

TEST_CASE("unformatted text") {
  SECTION("short line") {
    format_test("hello\n", "hello\n", no_fill);
  }

  SECTION("two short lines, one short output line") {
    format_test("hello\nworld\n", "hello world\n", no_fill);
  }

  SECTION("long enough to wrap") {
    format_test(
      "It is a truth universally acknowledged, that a "
      "single man in possession of a good fortune, must "
      "be in want of a wife.",
      "It is a truth universally acknowledged, that a single man in\n"
      "possession of a good fortune, must be in want of a wife.\n",
      no_fill);
  }
}

void format_test(
    std::string input,
    std::string expected,
    test_options opts) {
  auto in = std::istringstream { };
  in.str(input);

  auto out = std::ostringstream { };

  auto formatter = stiX::screen_formatter { in, out };
  formatter.format();

  auto output = out.str();
  REQUIRE(output == expected);
}