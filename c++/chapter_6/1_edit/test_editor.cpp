#include "../../testlib/testlib.hpp"
#include "../../lib/getline.hpp"

#include "editor.hpp"
#include <sstream>
#include <string>
using namespace std::string_literals;

void editor_test(std::string consoleIO);

TEST_CASE("Chapter 6 - edit - editor") {
  SECTION("= command") {
    editor_test(
      "=\n"
      ">0\n"
    );

    editor_test(
      "=\n"
      "=\n"
      ">0\n"
      ">0\n"
    );
  } // =
  
  SECTION("i command") {
    editor_test(
      "i\n"
      "Hello World!\n"
      "Woo!\n"
      ".\n"
      "=\n"
      ">2\n"
    );
    editor_test(
      "i\n"
      "Hello World!\n"
      ".\n"
      "=\n"
      ">1\n"
      "i\n"
      "Woo!\n"
      ".\n"
      "=\n"
      ">1\n"
    );
    editor_test(
      "=\n"
      ">0\n"
      "i\n"
      ".\n"
      "=\n"
      ">0\n"
    );
    editor_test(
      "i\n"
      "Line 1\n"
      "Line 2\n"
      ".\n"
      "1i\n"
      "Line 0\n"
      ".\n"
      "1,$p\n"
      ">Line 0\n"
      ">Line 1\n"
      ">Line 2\n"
    );
    editor_test(
      "i\n"
      "Line 1\n"
      "Line 2\n"
      ".\n"
      "2i\n"
      "Line 0\n"
      ".\n"
      "1,$p\n"
      ">Line 1\n"
      ">Line 0\n"
      ">Line 2\n"
    );
  } // i

  SECTION("p command") {
    editor_test(
      "p\n"
      ">?\n"
    );
    editor_test(
      "i\n"
      "Hello World!\n"
      "Hello Again\n"
      ".\n"
      "p\n"
      ">Hello Again\n"
    );
    editor_test(
      "i\n"
      "Hello World!\n"
      "Hello Again\n"
      ".\n"
      "1p\n"
      ">Hello World!\n"
    );
    editor_test(
      "i\n"
      "Hello World!\n"
      "Hello Again\n"
      ".\n"
      "1,2p\n"
      ">Hello World!\n"
      ">Hello Again\n"
    );
    editor_test(
      "i\n"
      "Hello World!\n"
      "Hello Again\n"
      ".\n"
      "1,$p\n"
      ">Hello World!\n"
      ">Hello Again\n"
    );
    editor_test(
      "i\n"
      "Hello World!\n"
      "Hello Again\n"
      ".\n"
      "1,$p\n"
      ">Hello World!\n"
      ">Hello Again\n"
    );
  }
}

std::string testLabel(std::string input);
auto const eof = std::char_traits<char>::eof();

void editor_test(std::string consoleIO) {
  DYNAMIC_SECTION("edit(" << testLabel(consoleIO) << ")") {
    auto console = std::istringstream(consoleIO);
    bool prevWasOutput = false;

    auto in = std::stringstream { };
    auto out = std::stringstream { };
    auto e = stiX::editor();

    while(console.peek() != eof) {
      auto line = stiX::getline(console);

      if (line[0] == '>') {
        if (!prevWasOutput) {
          out = std::stringstream{};
          e.process(in, out);
          in = std::stringstream{};
        }

        auto expected = line.substr(1);
        auto output = stiX::getline(out);

        REQUIRE(output == expected);
      } else {
        in << line << '\n';
      }

      prevWasOutput = line[0] == '>';
    }
  }
}

std::string testLabel(std::string input) {
  std::transform(input.begin(), input.end(), input.begin(), [](char ch) {
    return ch == '\n' ? ' ' : ch;
  });
  return input;
}
