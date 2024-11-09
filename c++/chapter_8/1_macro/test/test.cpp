#include "test.hpp"
#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "../macro.hpp"

void build_good_tests(std::vector<good_case> const& good) {
  SECTION("Happy") {
    for (auto g: good) {
      DYNAMIC_SECTION(g.name) {
        auto in = std::istringstream{g.input};
        auto out = std::ostringstream{};

        stiX::macro_process(in, out);

        REQUIRE(g.expected == out.str());
      }
    }
  }
}

void build_bad_tests(std::vector<bad_case> const& bad) {
  SECTION("Fails") {
    for (auto b: bad) {
      DYNAMIC_SECTION(b.input) {
        auto in = std::istringstream{b.input};
        auto out = std::ostringstream{};

        try {
          stiX::macro_process(in, out);
          FAIL("Expected failure, but succeeded");
        } catch (std::exception &ex) {
          REQUIRE(b.exception == ex.what());
        }
      }
    }
  }
}

void build_warning_tests(std::vector<warning_case> const& warnings) {
  SECTION("Warnings") {
    for (auto w: warnings) {
      DYNAMIC_SECTION(w.name) {
        auto in = std::istringstream{w.input};
        auto out = std::ostringstream{};
        auto err = std::ostringstream{};

        stiX::macro_process(in, out, err);

        REQUIRE(w.output == out.str());
        REQUIRE(w.warning == err.str());
      }
    }
  }
}
