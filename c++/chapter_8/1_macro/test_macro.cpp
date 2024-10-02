#include "../../testlib/testlib.hpp"
#include "macro.hpp"
#include <vector>

void fail_with(std::string const& input, std::string const& expected_exception) {
  auto in = std::istringstream { input };
  auto out = std::ostringstream { };

  try {
    stiX::macro_process(in, out);
    FAIL("Expected failure, but succeeded");
  } catch (std::exception& ex) {
    REQUIRE(expected_exception == ex.what());
  }
}

TEST_CASE("macro") {
  SECTION("pass through") {
    auto in = std::istringstream { "nothing going on here\nat all" };
    auto out = std::ostringstream { };

    stiX::macro_process(in, out);

    REQUIRE(out.str() == "nothing going on here\nat all");
  }

  SECTION("identify a definition") {
    auto in = std::istringstream { "nothing going on here\ndefine(x, y)\nat all" };
    auto out = std::ostringstream { };

    stiX::macro_process(in, out);

    REQUIRE(out.str() == "nothing going on here\n\nat all");
  }

  auto bad = std::vector<std::pair<std::string, std::string>> {
    { "define[x, y]", "Expected (" }
  };
  for (auto b : bad) {
    DYNAMIC_SECTION(b.first) {
      fail_with(b.first, b.second);
    }
  }
}