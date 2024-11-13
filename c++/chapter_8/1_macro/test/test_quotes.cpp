#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "test.hpp"

TEST_CASE("Quotes") {
  auto good = std::vector<good_case> {
    { "len($1) in replacement",
        "define(howlong, len($1)) howlong howlong(fruit)",
        " 2 2" },
    { "`len($1)' in replacement",
        "define(howlong, `len($1)') howlong howlong(fruit)",
        " 0 5" },
    { "len`($1)' in replacement",
      "define(howlong, len`($1)') howlong howlong(fruit)",
      " 0 5" },
    { "len(`$'1) in replacement",
      "define(howlong, len(`$'1)) howlong howlong(fruit)",
      " 2 2" },
    { "len(`$1') in replacement",
      "define(howlong, len(`$1')) howlong howlong(fruit)",
      " 2 2" },
    { "create an alias for define, use it",
      "define(def, `define($1,$2)') def(fish, fowl) fish",
      "  fowl" },
    { "whole expression is quoted",
      "`define($1,$2)'!",
      "define($1,$2)!" },
    { "self-updating macro",
      "define(`ACCEPT', `define(`ACCEPT',`Already accepted')Accepted')ACCEPT, ACCEPT",
      "Accepted, Already accepted" }
  };
  build_good_tests(good);
}
