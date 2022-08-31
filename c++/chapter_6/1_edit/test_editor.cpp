#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"

#include "editor.hpp"
#include <sstream>
#include <string>
using namespace std::string_literals;

TEST_CASE("Chapter 6 - edit - editor") {
  SECTION("new editor") {
    auto input = "=\n"s;
    auto output = "0\n"s;

    auto e = stiX::editor();

    auto instr = std::istringstream(input);
    auto ostr = std::ostringstream();

    e.process(instr, ostr);

    REQUIRE(ostr.str() == output);
  }
}