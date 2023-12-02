#define ADDITIONAL_TESTS

#include "command_actions.hpp"
#include "../../testlib/testlib.hpp"
#include "edit_buffer.hpp"

using namespace std::literals;

TEST_CASE("Chapter 6 - edit - command actions") {
  auto three_line_buffer = []() -> stiX::edit_buffer {
    auto eb = stiX::edit_buffer();
    eb.insert_before(eb.dot(), "line 1");
    eb.insert_before(eb.dot(), "line 2");
    eb.insert_before(eb.dot(), "line 3");
    return eb;
  };

  auto five_line_buffer = []() -> stiX::edit_buffer {
    auto eb = stiX::edit_buffer();
    eb.insert_before(eb.dot(), "line 1");
    eb.insert_before(eb.dot(), "line 2");
    eb.insert_before(eb.dot(), "line 3");
    eb.insert_before(eb.dot(), "line 4");
    eb.insert_before(eb.dot(), "line 5");
    return eb;
  };

  SECTION("delete action") {
    SECTION("delete first line") {
      auto e = three_line_buffer();

      delete_action(1, 1, e);

      REQUIRE(e.last() == 2);
      REQUIRE(e.line_at(1) == "line 2");
      REQUIRE(e.line_at(2) == "line 3");
    }
    SECTION("delete third line") {
      auto e = three_line_buffer();

      delete_action(3, 3, e);

      REQUIRE(e.last() == 2);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 2");
    }
    SECTION("delete line 1,2") {
      auto e = three_line_buffer();

      delete_action(1, 2, e);

      REQUIRE(e.dot() == 1);
      REQUIRE(e.last() == 1);
      REQUIRE(e.line_at(1) == "line 3");
    }
    SECTION("delete line 2,3") {
      auto e = three_line_buffer();

      delete_action(2, 3, e);

      REQUIRE(e.dot() == 1);
      REQUIRE(e.last() == 1);
      REQUIRE(e.line_at(1) == "line 1");
    }
    SECTION("delete line 1,3") {
      auto e = three_line_buffer();

      delete_action(1, 3, e);

      REQUIRE(e.dot() == 0);
      REQUIRE(e.last() == 0);
    }
  }

  SECTION("change action") {
    SECTION("change first line for nothing") {
      auto e = three_line_buffer();
      auto in = std::istringstream(".\n`");

      change_action(in, 1, 1, e);

      REQUIRE(e.last() == 2);
      REQUIRE(e.line_at(1) == "line 2");
      REQUIRE(e.line_at(2) == "line 3");
    }

    SECTION("change first line") {
      auto e = three_line_buffer();
      auto in = std::istringstream("hello\n.\n`");

      change_action(in, 1, 1, e);

      REQUIRE(e.last() == 3);
      REQUIRE(e.line_at(1) == "hello");
      REQUIRE(e.line_at(2) == "line 2");
      REQUIRE(e.line_at(3) == "line 3");
    }

    SECTION("change first for more") {
      auto e = three_line_buffer();
      auto in = std::istringstream("hello\nworld\n!\n.\n`");

      change_action(in, 1, 1, e);

      REQUIRE(e.last() == 5);
      REQUIRE(e.line_at(1) == "hello");
      REQUIRE(e.line_at(2) == "world");
      REQUIRE(e.line_at(3) == "!");
      REQUIRE(e.line_at(4) == "line 2");
      REQUIRE(e.line_at(5) == "line 3");
    }

    SECTION("change second line for nothing") {
      auto e = three_line_buffer();
      auto in = std::istringstream(".\n`");

      change_action(in, 2, 2, e);

      REQUIRE(e.last() == 2);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 3");
    }

    SECTION("change second line") {
      auto e = three_line_buffer();
      auto in = std::istringstream("hello\n.\n`");

      change_action(in, 2, 2, e);

      REQUIRE(e.last() == 3);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "hello");
      REQUIRE(e.line_at(3) == "line 3");
    }

    SECTION("change second for more") {
      auto e = three_line_buffer();
      auto in = std::istringstream("hello\nworld\n!\n.\n`");

      change_action(in, 2, 2, e);

      REQUIRE(e.last() == 5);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "hello");
      REQUIRE(e.line_at(3) == "world");
      REQUIRE(e.line_at(4) == "!");
      REQUIRE(e.line_at(5) == "line 3");
    }

    SECTION("change last line for nothing") {
      auto e = three_line_buffer();
      auto in = std::istringstream(".\n`");

      change_action(in, 3, 3, e);

      REQUIRE(e.last() == 2);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 2");
    }

    SECTION("change last line") {
      auto e = three_line_buffer();
      auto in = std::istringstream("hello\n.\n`");

      change_action(in, 3, 3, e);

      REQUIRE(e.last() == 3);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 2");
      REQUIRE(e.line_at(3) == "hello");
    }

    SECTION("change last for more") {
      auto e = three_line_buffer();
      auto in = std::istringstream("hello\nworld\n!\n.\n`");

      change_action(in, 3, 3, e);

      REQUIRE(e.last() == 5);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 2");
      REQUIRE(e.line_at(3) == "hello");
      REQUIRE(e.line_at(4) == "world");
      REQUIRE(e.line_at(5) == "!");
    }

    SECTION("change everything for more") {
      auto e = three_line_buffer();
      auto in = std::istringstream("hello\nBrian\nand\nPJ\n.\n`");

      change_action(in, 1, 3, e);

      REQUIRE(e.last() == 4);
      REQUIRE(e.line_at(1) == "hello");
      REQUIRE(e.line_at(2) == "Brian");
      REQUIRE(e.line_at(3) == "and");
      REQUIRE(e.line_at(4) == "PJ");
    }

    SECTION("change last two lines for more") {
      auto e = three_line_buffer();
      auto in = std::istringstream("hello\nBrian\nand\nPJ\n.\n`");

      change_action(in, 2, 3, e);

      REQUIRE(e.last() == 5);
      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "hello");
      REQUIRE(e.line_at(3) == "Brian");
      REQUIRE(e.line_at(4) == "and");
      REQUIRE(e.line_at(5) == "PJ");
    }
  }

  SECTION("move action") {
    SECTION("move 1 to $") {
      auto e = three_line_buffer();

      move_action(1, 1, 3, e);

      REQUIRE(e.dot() == 3);
      REQUIRE(e.line_at(1) == "line 2");
      REQUIRE(e.line_at(2) == "line 3");
      REQUIRE(e.line_at(3) == "line 1");
    }

    SECTION("move 1-2 to $") {
      auto e = three_line_buffer();

      move_action(1, 2, 3, e);

      REQUIRE(e.dot() == 3);
      REQUIRE(e.line_at(1) == "line 3");
      REQUIRE(e.line_at(2) == "line 1");
      REQUIRE(e.line_at(3) == "line 2");
    }

    SECTION("move 2-3 to 4 of 5") {
      auto e = five_line_buffer();

      move_action(2, 3, 4, e);

      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 4");
      REQUIRE(e.line_at(3) == "line 2");
      REQUIRE(e.line_at(4) == "line 3");
      REQUIRE(e.line_at(5) == "line 5");
    }

    SECTION("move 2-4 to 5 of 5") {
      auto e = five_line_buffer();

      move_action(2, 4, 5, e);

      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 5");
      REQUIRE(e.line_at(3) == "line 2");
      REQUIRE(e.line_at(4) == "line 3");
      REQUIRE(e.line_at(5) == "line 4");
    }

    SECTION("move 4-5 to 2 of 5") {
      auto e = five_line_buffer();

      move_action(4, 5, 2, e);

      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 4");
      REQUIRE(e.line_at(3) == "line 5");
      REQUIRE(e.line_at(4) == "line 2");
      REQUIRE(e.line_at(5) == "line 3");
    }

    SECTION("move, then move back") {
      auto e = five_line_buffer();

      move_action(4, 5, 2, e);
      move_action(2, 3, 5, e);

      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 2");
      REQUIRE(e.line_at(3) == "line 3");
      REQUIRE(e.line_at(4) == "line 4");
      REQUIRE(e.line_at(5) == "line 5");

      move_action(4, 5, 2, e);
      move_action(4, 5, 2, e);

      REQUIRE(e.line_at(1) == "line 1");
      REQUIRE(e.line_at(2) == "line 2");
      REQUIRE(e.line_at(3) == "line 3");
      REQUIRE(e.line_at(4) == "line 4");
      REQUIRE(e.line_at(5) == "line 5");
    }
  }
}