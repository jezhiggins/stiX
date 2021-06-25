#include "../../testlib/testlib.hpp"

#include "pattern_matcher.hpp"
#include "matcher.hpp"
#include <ranges>


TEST_CASE("Chapter 5 - find - single matcher") {
  SECTION("single character match") {
    auto m = stiX::make_matcher("a");
    REQUIRE(m.match('a'));
    REQUIRE_FALSE(m.match('b'));
  }
  SECTION("any character match") {
    auto m = stiX::make_matcher("?");
    REQUIRE(m.match('a'));
    REQUIRE(m.match('A'));
    REQUIRE(m.match('?'));
    REQUIRE(m.match(' '));
    REQUIRE_FALSE(m.match('\n'));
  }
  SECTION("multi-character match") {
    auto m = stiX::make_matcher("abc");
    REQUIRE(m.match('a'));
    REQUIRE(m.match('b'));
    REQUIRE(m.match('c'));
    REQUIRE_FALSE(m.match('v'));
  }
}

TEST_CASE("Chapter 5 - find - pattern matcher") {
  SECTION("compile patterns") {
    auto p = stiX::compile_pattern("h");
    REQUIRE(p.size() == 1);
    REQUIRE(p.match("hello"));
    REQUIRE_FALSE(p.match("goodbye"));
  }
}