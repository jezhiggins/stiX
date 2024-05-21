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

  SECTION("c command") {
    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      ".\n"
      "1c\n"
      "changed 1\n"
      ".\n"
      "1,$p\n"
      ">changed 1\n"
      ">line 2\n"
      ">line 3\n"
    );
    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      ".\n"
      "3c\n"
      "changed 3\n"
      "changed 4\n"
      ".\n"
      "1,$p\n"
      ">line 1\n"
      ">line 2\n"
      ">changed 3\n"
      ">changed 4\n"
    );
    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      ".\n"
      "1,$c\n"
      "changed 1\n"
      "changed 2\n"
      "changed 3\n"
      ".\n"
      "1,$p\n"
      ">changed 1\n"
      ">changed 2\n"
      ">changed 3\n"
    );
  } // c

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
    editor_test(
      "i\n"
      "line one\n"
      "line two\n"
      "line three\n"
      "line four\n"
      "line five\n"
      "line six\n"
      "line seven\n"
      ".\n"
      "4,5dp\n"
      ">line six\n"
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

  SECTION("m command") {
    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      ".\n"
      "2,3m5\n"
      "1,$p\n"
      ">line 1\n"
      ">line 4\n"
      ">line 5\n"
      ">line 2\n"
      ">line 3\n"
    );
    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      ".\n"
      "1,4m5\n"
      "1,$p\n"
      ">line 5\n"
      ">line 1\n"
      ">line 2\n"
      ">line 3\n"
      ">line 4\n"
    );
  }

  SECTION("p command") {
    editor_test(
      "p\n"
      ">?\n"
    );
    editor_test(
      "1p\n"
      ">?\n"
    );
    editor_test(
      "0p\n"
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
      "2p\n"
      ">Hello Again\n"
    );
    editor_test(
      "i\n"
      "Hello World!\n"
      "Hello Again\n"
      ".\n"
      "1p\n"
      ">Hello World!\n"
      "\n"
      ">Hello Again\n"
      "\n"
      ">?\n"
      "=\n"
      ">2\n"
    );
    editor_test(
      "i\n"
      "Hello World!\n"
      "Hello Again\n"
      ".\n"
      "0p\n"
      ">?\n"
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
      "Hello World!\n"
      "Hello Again\n"
      ".\n"
      "/Again/p\n"
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
      "1,.-1p\n"
      ">line one\n"
      ">line two\n"
      "1,.+2p\n"
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
      "1p\n"
      ">line one\n"
      "\n"
      ">line two\n"
      "\n"
      ">line three\n"
      "\n"
      ">line four\n"
      "\n"
      ">line five\n"
      "\n"
      ">?\n"
    );
  }

  SECTION("s command") {
    editor_test(
      "i\n"
      "line one\n"
      "line two\n"
      "line three\n"
      "line four\n"
      "line five\n"
      ".\n"
      "1,$s/line/Line/\n"
      "1,$p\n"
      ">Line one\n"
      ">Line two\n"
      ">Line three\n"
      ">Line four\n"
      ">Line five\n"
    );
    editor_test(
      "i\n"
      "line one\n"
      "line two\n"
      "line three\n"
      "line four\n"
      "line five\n"
      ".\n"
      "1,$s/e/E/\n"
      "1,$p\n"
      ">linE one\n"
      ">linE two\n"
      ">linE three\n"
      ">linE four\n"
      ">linE five\n"
    );
    editor_test(
      "i\n"
      "line one\n"
      "line two\n"
      "line three\n"
      "line four\n"
      "line five\n"
      ".\n"
      "1,$s/e/E/g\n"
      "1,$p\n"
      ">linE onE\n"
      ">linE two\n"
      ">linE thrEE\n"
      ">linE four\n"
      ">linE fivE\n"
    );
    editor_test(
      "i\n"
      "line one\n"
      "line two\n"
      "line three\n"
      "line four\n"
      "line five\n"
      ".\n"
      "1,$s/[aeiou]//g\n"
      "1,$p\n"
      ">ln n\n"
      ">ln tw\n"
      ">ln thr\n"
      ">ln fr\n"
      ">ln fv\n"
    );
  }

  SECTION("g command") {
    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      ".\n"
      "1,$g/%/m0\n"
      "1,$p\n"
      ">line 5\n"
      ">line 4\n"
      ">line 3\n"
      ">line 2\n"
      ">line 1\n"
    );

    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      ".\n"
      "g/%/m0\n"
      "1,$p\n"
      ">line 5\n"
      ">line 4\n"
      ">line 3\n"
      ">line 2\n"
      ">line 1\n"
    );

    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      "line 6\n"
      "line 7\n"
      ".\n"
      "g/[246]/m0\n"
      "1,$p\n"
      ">line 6\n"
      ">line 4\n"
      ">line 2\n"
      ">line 1\n"
      ">line 3\n"
      ">line 5\n"
      ">line 7\n"
    );

    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      "line 6\n"
      "line 7\n"
      ".\n"
      "g/[246]/m$\n"
      "1,$p\n"
      ">line 1\n"
      ">line 3\n"
      ">line 5\n"
      ">line 7\n"
      ">line 2\n"
      ">line 4\n"
      ">line 6\n"
    );

    editor_test(
      "g/^/c\n"
      ">?\n"
    );

    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      "line 6\n"
      "line 7\n"
      ".\n"
      "g/[246]/.,.+2m0\n"
      ">?\n"
      "1,$p\n"
      ">line 4\n"
      ">line 1\n"
      ">line 5\n"
      ">line 2\n"
      ">line 3\n"
      ">line 6\n"
      ">line 7\n"
    );

    editor_test(
      "i\n"
      "line 0\n"
      "line 0\n"
      "line 0\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      "line 6\n"
      "line 7\n"
      ".\n"
      "g/[246]/.,.+2m0\n"
      ">?\n"
      "1,$p\n"
      ">line 2\n"
      ">line 3\n"
      ">line 4\n"
      ">line 0\n"
      ">line 0\n"
      ">line 0\n"
      ">line 1\n"
      ">line 5\n"
      ">line 6\n"
      ">line 7\n"
    );

    editor_test(
      "i\n"
      "line 0\n"
      "line 0\n"
      "line 0\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      "line 6\n"
      "line 7\n"
      ".\n"
      "6,$g/[246]/.,.+2m0\n"
      "1,$p\n"
      ">line 4\n"
      ">line 5\n"
      ">line 6\n"
      ">line 0\n"
      ">line 0\n"
      ">line 0\n"
      ">line 1\n"
      ">line 2\n"
      ">line 3\n"
      ">line 7\n"
    );

    editor_test(
      "i\n"
      "line 1\n"
      "line 2\n"
      "line 3\n"
      "line 4\n"
      "line 5\n"
      "line 6\n"
      "line 7\n"
      ".\n"
      "g/[246]/.+5p\n"
      ">line 7\n"
      ">?\n"
      "1p\n"
      ">line 1\n"
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
