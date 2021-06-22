#include "../../testlib/testlib.hpp"

#include "matcher.hpp"

TEST_CASE("Chapter 5 - find - matcher") {
  SECTION("single character match") {
    auto m = stiX::make_matcher("a");
    REQUIRE(m.match('a'));
    REQUIRE_FALSE(m.match('b'));
  }
  SECTION("multi-character match") {
    auto m = stiX::make_matcher("abc");
    REQUIRE(m.match('a'));
    REQUIRE(m.match('b'));
    REQUIRE(m.match('c'));
    REQUIRE_FALSE(m.match('v'));
  }
}
