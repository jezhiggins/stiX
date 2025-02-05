#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "test.hpp"

TEST_CASE("changeq()") {
  auto bad = std::vector<bad_case> {
    { "changeq(pants", "Expected )" }
  };

  build_bad_tests(bad);
}