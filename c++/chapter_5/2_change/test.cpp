#include "../../testlib/testlib.hpp"

#include "../../lib/regex/pattern_matcher.hpp"

TEST_CASE("Chapter 5 - change - pattern match location") {
  SECTION("no match") {
    auto p = stiX::compile_pattern("p");

    auto r = p.find("hello");
    REQUIRE_FALSE(r.match);
    REQUIRE(r.start == std::string::npos);
    REQUIRE(r.start == r.end);
  }

  SECTION("single-char match at start of line") {
    auto p = stiX::compile_pattern("h");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.start == 0);
    REQUIRE(r.end == 1);
  }

  SECTION("single-char match at end of line") {
    auto p = stiX::compile_pattern("o");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.start == 4);
    REQUIRE(r.end == 5);
  }

  SECTION("match at start of line") {
    auto p = stiX::compile_pattern("%hell");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.start == 0);
    REQUIRE(r.end == 4);
  }

  SECTION("match at end of line") {
    auto p = stiX::compile_pattern("ello$");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.start == 1);
    REQUIRE(r.end == 5);
  }

  SECTION("match in middle of line") {
    auto p = stiX::compile_pattern("ell");

    auto r = p.find("hello");
    REQUIRE(r.match);
    REQUIRE(r.start == 1);
    REQUIRE(r.end == 4);
  }
}