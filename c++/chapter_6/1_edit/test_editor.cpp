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

  SECTION("a command") {
    editor_test(
      "a\n"
      "Hello World!\n"
      "Woo!\n"
      ".\n"
      "=\n"
      ">2\n"
      "1,$p\n"
      ">Hello World!\n"
      ">Woo!\n"
    );
    editor_test(
      "a\n"
      "Hello World!\n"
      ".\n"
      "=\n"
      ">1\n"
      "a\n"
      "Woo!\n"
      ".\n"
      "=\n"
      ">2\n"
      "1,$p\n"
      ">Hello World!\n"
      ">Woo!\n"
    );
    editor_test(
      "=\n"
      ">0\n"
      "a\n"
      ".\n"
      "=\n"
      ">0\n"
    );
    editor_test(
      "i\n"
      "Line 1\n"
      "Line 2\n"
      ".\n"
      "2a\n"
      "Line 3\n"
      ".\n"
      "1,$p\n"
      ">Line 1\n"
      ">Line 2\n"
      ">Line 3\n"
    );
  }

  SECTION("i command") {
    editor_test(
      "i\n"
      "Hello World!\n"
      "Woo!\n"
      ".\n"
      "=\n"
      ">2\n"
      "1,$p\n"
      ">Hello World!\n"
      ">Woo!\n"
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
      "1,$p\n"
      ">Woo!\n"
      ">Hello World!\n"
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
      "Line 1.5\n"
      ".\n"
      "1,$p\n"
      ">Line 1\n"
      ">Line 1.5\n"
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
    editor_test(
      "i\n"
      "line one\n"
      "line two\n"
      "line three\n"
      "line four\n"
      "line five\n"
      ".\n"
      "=\n"
      ">5\n"
      "1,$-4p\n"
      ">line one\n"
      "1,$-2p\n"
      ">line one\n"
      ">line two\n"
      ">line three\n"
      "1,.-3p\n"
      ">line one\n"
      ">line two\n"
      "1,.-1p\n"
      ">line one\n"
      ">line two\n"
      ">line three\n"
      ">line four\n"
      "$-2,$p\n"
      ">line three\n"
      ">line four\n"
      ">line five\n"
      ".,$p\n"
      ">line five\n"
      "$,$p\n"
      ">line five\n"
      ".,.p\n"
      ">line five\n"
    );
  }
  SECTION("d command") {
    editor_test(
      "i\n"
      "line one\n"
      "line two\n"
      "line three\n"
      "line four\n"
      "line five\n"
      ".\n"
      "=\n"
      ">5\n"
      "3d\n"
      "=\n"
      ">3\n"
      "1,$p\n"
      ">line one\n"
      ">line two\n"
      ">line four\n"
      ">line five\n"
    );
    editor_test(
      "i\n"
      "line one\n"
      "line two\n"
      "line three\n"
      "line four\n"
      "line five\n"
      ".\n"
      "=\n"
      ">5\n"
      "2,4d\n"
      "=\n"
      ">2\n"
      "1,$p\n"
      ">line one\n"
      ">line five\n"
    );
  }
}

std::string testLabel(std::string input);
auto const eof = std::char_traits<char>::eof();

void editor_test(std::string consoleIO) {
  DYNAMIC_SECTION("edit(" << testLabel(consoleIO) << ")") {
    auto console = std::istringstream(consoleIO);
    bool prevWasOutput = false;

    auto log = std::ostringstream { };

    auto in = std::stringstream { };
    auto out = std::stringstream { };
    auto e = stiX::editor();

    while(console.peek() != eof) {
      auto line = stiX::getline(console);
      log << line << '\n';

      if (line[0] == '>') {
        if (!prevWasOutput) {
          out = std::stringstream{};
          e.process(in, out);
          in = std::stringstream{};
        }

        auto expected = line.substr(1);
        auto output = stiX::getline(out);

        INFO(log.str());
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
