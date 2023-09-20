#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"

#include "line_buffer.hpp"

TEST_CASE("Chapter 6 - edit - line_buffer") {
  SECTION("empty buffer") {
    auto b = stiX::line_buffer();

    REQUIRE(b.size() == 0);
  }

  SECTION("insert a line before 0") {
    auto b = stiX::line_buffer();
    b.insert(0, "a line");

    REQUIRE(b.size() == 1);
    REQUIRE(b[0] == "a line");
  }

  SECTION("insert a line before 0, then another") {
    auto b = stiX::line_buffer();
    b.insert(0, "line 1");
    b.insert(0, "line 2");

    REQUIRE(b.size() == 2);
    REQUIRE(b[0] == "line 2");
    REQUIRE(b[1] == "line 1");
  }

  SECTION("insert a line before end, then another") {
    auto b = stiX::line_buffer();
    b.insert(b.size(), "line 1");
    b.insert(b.size(), "line 2");

    REQUIRE(b.size() == 2);
    REQUIRE(b[0] == "line 1");
    REQUIRE(b[1] == "line 2");
  }

  SECTION("insert a line before end, then at the start, then at the end") {
    auto b = stiX::line_buffer();
    b.insert(b.size(), "line 1");
    b.insert(0, "line 2");
    b.insert(b.size(), "line 3");

    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == "line 2");
    REQUIRE(b[1] == "line 1");
    REQUIRE(b[2] == "line 3");
  }

  SECTION("insert a line before end, then at the start, then before the second line") {
    auto b = stiX::line_buffer();
    b.insert(b.size(), "line 1");
    b.insert(0, "line 2");
    b.insert(1, "line 3");

    REQUIRE(b.size() == 3);
    REQUIRE(b[0] == "line 2");
    REQUIRE(b[1] == "line 3");
    REQUIRE(b[2] == "line 1");
  }

  SECTION("three line buffer, delete first line") {
    auto b = stiX::line_buffer();
    b.insert(b.size(), "line 1");
    b.insert(b.size(), "line 2");
    b.insert(b.size(), "line 3");

    b.remove(0);

    REQUIRE(b.size() == 2);
    REQUIRE(b[0] == "line 2");
    REQUIRE(b[1] == "line 3");
  }

  SECTION("three line buffer, delete last line") {
    auto b = stiX::line_buffer();
    b.insert(b.size(), "line 1");
    b.insert(b.size(), "line 2");
    b.insert(b.size(), "line 3");

    b.remove(2);

    REQUIRE(b.size() == 2);
    REQUIRE(b[0] == "line 1");
    REQUIRE(b[1] == "line 2");
  }
}
