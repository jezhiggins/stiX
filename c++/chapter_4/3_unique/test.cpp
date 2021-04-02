#include "../../testlib/testlib.h"
#include "unique.h"

using lines_t = std::vector<std::string>;
void testUnique(lines_t const& input, lines_t const& expected);

TEST_CASE("Chapter 4 - unique") {
  SECTION("no input") {
    testUnique({}, {});
  }

  SECTION("one line") {
    testUnique({"hello"}, {"hello"});
  }

  SECTION("two different lines") {
    auto input = lines_t { "hello", "world" };
    testUnique(input, input);
  }

  SECTION("two identical lines") {
    testUnique({ "hello", "hello" }, { "hello" });
  }

  SECTION("three identical lines") {
    testUnique({ "hello", "hello", "hello" }, { "hello" });
  }

  SECTION("multiple different lines") {
    auto mary = lines_t {
      "mary had a little lamb",
      "it's fleece was white as snow",
      "and everywhere that Mary went",
      "the lamb was sure to go"
    };
    testUnique(mary, mary);
  }

  SECTION("multiple lines with repeats") {
    auto marymary = lines_t {
      "mary had a little lamb",
      "mary had a little lamb",
      "mary had a little lamb",
      "it's fleece was white as snow",
      "it's fleece was white as snow",
      "and everywhere that Mary went",
      "the lamb was sure to go",
      "the lamb was sure to go",
      "the lamb was sure to go",
      "the lamb was sure to go",
    };
    auto mary = lines_t {
      "mary had a little lamb",
      "it's fleece was white as snow",
      "and everywhere that Mary went",
      "the lamb was sure to go"
    };
    testUnique(marymary, mary);
  }
  SECTION("multiple lines with blanks") {
    auto blonk = lines_t {
      "",
      "",
      "",
      "the lamb was sure to go",
    };
    auto stripped = lines_t {
      "",
      "the lamb was sure to go",
    };
    testUnique(blonk, stripped);
  }
}

std::string join(lines_t const& lines) {
  auto s = std::ostringstream { };
  for (auto const& line : lines)
    s << line << "\n";
  return s.str();
}

void testUnique(lines_t const& input, lines_t const& expected) {
  auto all_input = join(input);
  auto all_output = join(expected);
  testFilter("unique", join(input), join(expected), stiX::unique);
}
