#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"

#include "editor.hpp"

TEST_CASE("Chapter 6 - edit - editor") {
  SECTION("new editor") {
    auto e = stiX::editor();

    REQUIRE(e.dot() == 0);
    REQUIRE(e.last() == 0);
  }
}