#include "../../testlib/testlib.hpp"

#include "edit_buffer.hpp"

TEST_CASE("Chapter 6 - edit - edit_buffer") {
  SECTION("empty buffer") {
    auto b = stiX::edit_buffer();

    REQUIRE(b.size() == 0);
  }
}