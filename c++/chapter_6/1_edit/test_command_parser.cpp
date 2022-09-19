#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"

#include "command_parser.hpp"

TEST_CASE("Chapter 6 - edit - command parser") {
  SECTION("no input, empty buffer") {
    auto command = stiX::parse_command("", 0, 0);

    REQUIRE(command.from_index == 0);
    REQUIRE(command.to_index == 0);
    REQUIRE(command.code == '\n');
  }

  SECTION("no input, dot is set") {
    auto command = stiX::parse_command("", 3, 5);

    REQUIRE(command.from_index == 3);
    REQUIRE(command.to_index == 3);
    REQUIRE(command.code == '\n');
  }
}
