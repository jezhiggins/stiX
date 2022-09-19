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
      "=\n",
      "0\n"
    );

    editor_test(
      "=\n=\n",
      "0\n0\n"
    );
  } // =

  SECTION("i command") {
    editor_test(
      "i\nHello World!\nWoo!\n.\n=\n",
      "2\n"
    );
    editor_test(
      "i\nHello World!\n.\n=\ni\nWoo!\n.\n=\n",
      "1\n2\n"
    );
    editor_test(
      "=\ni\n.\n=\n",
      "0\n0\n"
    );
  } // i

  SECTION("p command") {
    editor_test(
      "p\n",
      "?\n"
    );
    editor_test(
      "i\nHello World!\nHello Again\n.\np\n",
      "Hello Again\n"
    );
  }
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
