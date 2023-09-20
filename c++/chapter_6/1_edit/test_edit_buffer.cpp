#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"

#include "edit_buffer.hpp"

auto three_line_buffer() -> stiX::edit_buffer {
  auto eb = stiX::edit_buffer();
  eb.insert_before(eb.dot(), "line 1");
  eb.insert_before(eb.dot(), "line 2");
  eb.insert_before(eb.dot(), "line 3");
  return eb;
}

TEST_CASE("Chapter 6 - edit - edit_buffer") {
  SECTION("new edit_buffer") {
    auto e = stiX::edit_buffer();

    REQUIRE(e.dot() == 0);
    REQUIRE(e.last() == 0);
  }

  SECTION("insert") {
    SECTION("insert at .") {
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
  SECTION("delete") {
    auto three_line_buffer = []() -> stiX::edit_buffer {
      auto eb = stiX::edit_buffer();
      eb.insert_before(eb.dot(), "line 1");
      eb.insert_before(eb.dot(), "line 2");
      eb.insert_before(eb.dot(), "line 3");
      return eb;
    };

    SECTION("delete the first line") {
      auto e = three_line_buffer();
      e.remove_at(1);

      REQUIRE(e.dot() == 1);
      REQUIRE(e.last() == 2);
      REQUIRE(e.line_at(1) == "line 2");
      REQUIRE(e.line_at(2) == "line 3");
    }

    SECTION("delete line 2") {
      auto e = three_line_buffer();
      e.remove_at(2);

      REQUIRE(e.dot() == 2);
      REQUIRE(e.last() == 2);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 3");
    }

    SECTION("delete the last line") {
      auto e = three_line_buffer();
      e.remove_at(3);

      REQUIRE(e.dot() == 2);
      REQUIRE(e.last() == 2);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 2");
    }
  }
}