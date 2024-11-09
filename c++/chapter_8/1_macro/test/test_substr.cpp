#define ADDITIONAL_TESTS
#include "../../../testlib/testlib.hpp"
#include "test.hpp"

TEST_CASE("substr()") {
  auto good = std::vector<good_case> {
    { "substr(s,0,n)", "substr(Brian Kernighan, 0, 5)", "Brian" },
    { "substr(s,m,n)", "substr(PJ Plauger, 3, 7)", "Plauger" },
    { "substr(s,0)", "substr(Brian Kernighan, 0)", "Brian Kernighan" },
    { "substr(s,m)", "substr(PJ Plauger, 3)", "Plauger" },
    { "substr m is off end", "substr(Hello, 99, 50)", "" },
    { "no arg", "substr is the macro", "substr is the macro" },
    { "with define string arg", "define(f, hello world)substr(f, 3, 5)", "lo wo" },
    { "with define numeric arg", "define(f, 2)substr(hello, 0, f)", "he" },
  };
  auto warning = std::vector<warning_case> {
    { "non-numeric", "substr(hello, f)", "", "Warning: non-numeric argument to `substr'\n" },
    { "empty args", "substr()", "", "Warning: too few arguments to `substr'\n" },
    { "too few args", "substr(hello)", "hello", "Warning: too few arguments to `substr'\n" },
    { "excess args", "substr(Big pig, 4, 3, 99)", "pig", "Warning: excess arguments to `substr' ignored\n" },
  };

  build_good_tests(good);
  build_warning_tests(warning);
}