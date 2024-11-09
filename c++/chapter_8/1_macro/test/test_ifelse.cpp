#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "test.hpp"

TEST_CASE("ifelse()") {
  auto good = std::vector<good_case> {
    { "then", "ifelse(a, a, yes, no)", "yes" },
    { "else", "ifelse(a, b, yes, no)", "no" },
    { "then with replacement", "define(pet, dog)ifelse(pet,dog,it's a dog,oh no)", "it's a dog" },
    { "else with replacement", "define(pet, crocodile)ifelse(pet,dog,it's a dog,oh no)", "oh no" },
    { "no arg", "ifelse is the macro", "ifelse is the macro" }
  };
  auto warning = std::vector<warning_case> {
    { "then excess args", "ifelse(a,a,yes,no,whatever)", "yes", "Warning: excess arguments to `ifelse' ignored\n" },
  };

  build_good_tests(good);
  build_warning_tests(warning);
}