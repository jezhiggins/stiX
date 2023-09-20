#define ADDITIONAL_TESTS

#include "command_actions.hpp"
#include "../../testlib/testlib.hpp"
#include "edit_buffer.hpp"

TEST_CASE("Chapter 6 - edit - command actions") {
  auto three_line_buffer = []() -> stiX::edit_buffer {
    auto eb = stiX::edit_buffer();
    eb.insert_before(eb.dot(), "line 1");
    eb.insert_before(eb.dot(), "line 2");
    eb.insert_before(eb.dot(), "line 3");
    return eb;
  };

  SECTION("delete action") {
    auto e = three_line_buffer();

    stiX::delete_action(1, 1, e);

    REQUIRE(e.last() == 2);
    REQUIRE(e.line_at(1) == "line 2");
    REQUIRE(e.line_at(2) == "line 3");
  }
}