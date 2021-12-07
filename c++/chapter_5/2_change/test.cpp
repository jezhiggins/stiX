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
}