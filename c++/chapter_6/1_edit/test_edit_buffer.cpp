#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"

#include "edit_buffer.hpp"

TEST_CASE("Chapter 6 - edit - edit_buffer") {
  SECTION("new edit_buffer") {
    auto e = stiX::edit_buffer();

    REQUIRE(e.dot() == 0);
    REQUIRE(e.last() == 0);
  }

  SECTION("insert first line") {
    auto e = stiX::edit_buffer();
    e.insert_before(0, "a line");

    REQUIRE(e.dot() == 1);
    REQUIRE(e.last() == 1);
  }

  SECTION("insert two lines at .") {
    auto e = stiX::edit_buffer();
    e.insert_before(e.dot(), "line 1");
    e.insert_before(e.dot(), "line 2");

    REQUIRE(e.dot() == 2);
    REQUIRE(e.last() == 2);
  }
}