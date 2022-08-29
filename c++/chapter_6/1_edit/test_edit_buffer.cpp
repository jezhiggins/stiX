#include "../../testlib/testlib.hpp"

#include "edit_buffer.hpp"

TEST_CASE("Chapter 6 - edit - edit_buffer") {
  SECTION("empty buffer") {
    auto b = stiX::edit_buffer();

    REQUIRE(b.size() == 0);
  }

  SECTION("insert a line before  1") {
    auto b = stiX::edit_buffer();
    b.insert(0, "a line");

    REQUIRE(b.size() == 1);
    REQUIRE(b.at(0) == "a line");
  }

  SECTION("insert a line before 1, and then another") {
    auto b = stiX::edit_buffer();
    b.insert(0, "line 1");
    b.insert(0, "line 2");

    REQUIRE(b.size() == 2);
    REQUIRE(b.at(0) == "line 2");
    REQUIRE(b.at(1) == "line 1");
  }

  SECTION("insert a line before end, then another") {
    auto b = stiX::edit_buffer();
    b.insert(b.size(), "line 1");
    b.insert(b.size(), "line 2");

    REQUIRE(b.size() == 2);
    REQUIRE(b.at(0) == "line 1");
    REQUIRE(b.at(1) == "line 2");
  }
}
