#include "../../testlib/testlib.hpp"

#include "edit_buffer.hpp"

TEST_CASE("Chapter 6 - edit - edit_buffer") {
  SECTION("empty buffer") {
    auto b = stiX::edit_buffer();

    REQUIRE(b.size() == 0);
  }

  SECTION("insert a line before position 1") {
    auto b = stiX::edit_buffer();
    b.insert(1, "a line");

    REQUIRE(b.size() == 1);
  }
}