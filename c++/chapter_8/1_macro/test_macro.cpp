#include "../../testlib/testlib.hpp"
#include "macro.hpp"
#include <vector>

TEST_CASE("macro") {
  SECTION("pass through") {
    auto in = std::istringstream{"nothing going on here\nat all"};
    auto out = std::ostringstream{};

    stiX::macro_process(in, out);

    REQUIRE(out.str() == "nothing going on here\nat all");
  }

  SECTION("identify a definition") {
    auto in = std::istringstream{"nothing going on here\ndefine(x, y)\nat all"};
    auto out = std::ostringstream{};

    stiX::macro_process(in, out);

    REQUIRE(out.str() == "nothing going on here\n\nat all");
  }
}

TEST_CASE("Bad macros") {
  auto bad = std::vector<std::pair<std::string, std::string>> {
    { "define[x, y]", "Expected (" },
    { "define", "Expected (" },
    { "define(x:y)", "Expected ," },
    { "define(x, y]", "Expected )" },
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