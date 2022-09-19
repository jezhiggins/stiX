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

  SECTION("single letter") {
    auto command = stiX::parse_command("i", 3, 5);

    REQUIRE(command.from_index == 3);
    REQUIRE(command.to_index == 3);
    REQUIRE(command.code == 'i');
  }

  SECTION("1p") {
    auto command = stiX::parse_command("1p", 3, 5);

    REQUIRE(command.from_index == 1);
    REQUIRE(command.to_index == 1);
    REQUIRE(command.code == 'p');
  }
  SECTION(".p") {
    auto command = stiX::parse_command(".p", 3, 5);

    REQUIRE(command.from_index == 3);
    REQUIRE(command.to_index == 3);
    REQUIRE(command.code == 'p');
  }
  SECTION("$p") {
    auto command = stiX::parse_command("$p", 3, 5);

    REQUIRE(command.from_index == 5);
    REQUIRE(command.to_index == 5);
    REQUIRE(command.code == 'p');
  }

  SECTION("1,$p") {
    auto command = stiX::parse_command("1,$p", 3, 5);

    REQUIRE(command.from_index == 1);
    REQUIRE(command.to_index == 5);
    REQUIRE(command.code == 'p');
  }
  SECTION(".,4p") {
    auto command = stiX::parse_command(".,4p", 3, 5);

    REQUIRE(command.from_index == 3);
    REQUIRE(command.to_index == 4);
    REQUIRE(command.code == 'p');
  }
  SECTION("1,.p") {
    auto command = stiX::parse_command("1,.p", 3, 5);

    REQUIRE(command.from_index == 1);
    REQUIRE(command.to_index == 3);
    REQUIRE(command.code == 'p');
  }
}
