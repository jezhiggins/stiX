#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"

#include "editor.hpp"
#include <sstream>
#include <string>
using namespace std::string_literals;

void editor_test(std::string input, std::string expected);

TEST_CASE("Chapter 6 - edit - editor") {
  SECTION("= command") {
    editor_test(
      "=\n"s,
      "0\n"s
    );

    editor_test(
      "=\n=\n"s,
      "0\n0\n"s
    );
  } // =

  SECTION("i command") {
    editor_test(
      "i\nHello World!\nWoo!\n.\n=\n"s,
      "2\n"
    );
    editor_test(
      "i\nHello World!\n.\n=\ni\nWoo!\n.\n=\n"s,
      "1\n2\n"
    );
  } // i
}

std::string testLabel(std::string input);

void editor_test(std::string input, std::string expected) {
  DYNAMIC_SECTION("edit(" << testLabel(input) << ")") {
    auto e = stiX::editor();
    auto is = std::istringstream(input);
    auto output = std::ostringstream();

    e.process(is, output);

    REQUIRE(output.str() == expected);
  }
}

std::string testLabel(std::string input) {
  std::transform(input.begin(), input.end(), input.begin(), [](char ch) {
    return ch == '\n' ? ' ' : ch;
  });
  return input;
}
