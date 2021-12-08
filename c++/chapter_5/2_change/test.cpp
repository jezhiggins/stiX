#include "../../testlib/testlib.hpp"

#include "change.hpp"
#include "../../lib/regex/pattern_matcher.hpp"

TEST_CASE("Chapter 5 - change - pattern match location") {
  SECTION("no match") {
    auto p = stiX::compile_pattern("p");

    auto r = p.find("hello");
    REQUIRE_FALSE(r.match);
    REQUIRE(r.from == std::string::npos);
    REQUIRE(r.from == r.to);
  }

  SECTION("single-char match at start of line") {
    auto p = stiX::compile_pattern("h");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 1);
  }

  SECTION("single-char match at end of line") {
    auto p = stiX::compile_pattern("o");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 4);
    REQUIRE(r.to == 5);
  }

  SECTION("match at start of line") {
    auto p = stiX::compile_pattern("%hell");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 4);
  }

  SECTION("match at end of line") {
    auto p = stiX::compile_pattern("ello$");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 1);
    REQUIRE(r.to == 5);
  }

  SECTION("match in middle of line") {
    auto p = stiX::compile_pattern("ell");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 1);
    REQUIRE(r.to == 4);
  }

  SECTION("wildcard match at start of line") {
    auto p = stiX::compile_pattern("hel*");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 4);
  }

  SECTION("wildcard match at end of line") {
    auto p = stiX::compile_pattern("el*o");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 1);
    REQUIRE(r.to == 5);
  }

  SECTION("multiple wildcard match") {
    auto p = stiX::compile_pattern("h*e*l*o*");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 5);
  }

  SECTION("whole line match") {
    auto p = stiX::compile_pattern("%?*$");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 5);
  }

  SECTION("empty line match") {
    auto p = stiX::compile_pattern("%$");

    auto r = p.find("");
    REQUIRE(r.match);
    REQUIRE(r.from == 0);
    REQUIRE(r.to == 0);
  }
}

void testChange(
  std::string const& input,
  std::string const& expected,
  std::string const& pattern,
  std::string const& replacement
);

TEST_CASE("Chapter 5 - change") {
  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "hello world\nsit down\nhave a biscuit\n",
    "123",
    "");

  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "hello world\nsit down\nhave a digestive\n",
    "biscuit",
    "digestive");

  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "hll wrld\nst dwn\nhv  bsct\n",
    "[aeiou]",
    "");

  testChange(
    "hello world\nsit down\nhave a biscuit\n",
    "Hello world\nsit down\nHave a biscuit\n",
    "%h",
    "H");
}


void testChange(
  std::string const& input,
  std::string const& expected,
  std::string const& pattern,
  std::string const& replacement
) {
  testFilter("change", input, expected,
             [pattern, replacement](std::istream& in, std::ostream& out) {
               stiX::change(in, out, pattern, replacement);
             }
  );
}