#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"
#include "format_functions.hpp"

TEST_CASE("fill line") {
  REQUIRE(stiX::fill_line("hello world", 12) == "hello  world");
  REQUIRE(stiX::fill_line("big fat pig", 12) == "big  fat pig");
  REQUIRE(stiX::fill_line("big fat pig", 13) == "big  fat  pig");
  REQUIRE(stiX::fill_line("big fat pig", 14) == "big  fat   pig");
  REQUIRE(stiX::fill_line("big fat pig", 15) == "big   fat   pig");
  REQUIRE(stiX::fill_line("big fat pig", 16) == "big    fat   pig");
}