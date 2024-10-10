#include "../../testlib/testlib.hpp"
#include "macro.hpp"
#include <vector>

struct good_case {
  std::string name;
  std::string input;
  std::string expected;
};

TEST_CASE("macro") {
  auto good = std::vector<good_case> {
    { "pass through", "nothing going on here\nat all", "nothing going on here\nat all" },
    { "simple define", "nothing going on here\ndefine(x, y)\nat all", "nothing going on here\n\nat all"},
    { "define with parenthesised replacement",
      "nothing going on here\ndefine(ENDFILE, (-1))\nat all",
      "nothing going on here\n\nat all" },
    { "simple replacement",
      "define(DONE, ENDFILE)\nif (getit(line) = DONE) then putit(sumline)",
      "\nif (getit(line) = ENDFILE) then putit(sumline)"}
  };
  for (auto g : good) {
    DYNAMIC_SECTION(g.name) {
      auto in = std::istringstream{g.input};
      auto out = std::ostringstream{};

      stiX::macro_process(in, out);

      REQUIRE(g.expected == out.str());
    }
  }
}

TEST_CASE("Bad macros") {
  auto bad = std::vector<std::pair<std::string, std::string>> {
    { "define[x, y]", "Expected (" },
    { "define", "Expected (" },
    { "define(x:y)", "Expected ," },
    { "define(x, y]", "Expected )" },
    { "define(x, (((y))", "Expected )" },
    { "define(99, x)", "99 is not alphanumeric" }
  };
  for (auto b : bad) {
    DYNAMIC_SECTION(b.first) {
      auto in = std::istringstream { b.first };
      auto out = std::ostringstream { };

      try {
        stiX::macro_process(in, out);
        FAIL("Expected failure, but succeeded");
      } catch (std::exception& ex) {
        REQUIRE(b.second == ex.what());
      }
    }
  }
}