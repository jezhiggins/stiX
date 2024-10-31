#include "../../testlib/testlib.hpp"
#include "macro.hpp"
#include <vector>

struct good_case {
  std::string name;
  std::string input;
  std::string expected;
};

void build_good_tests(std::vector<good_case> const& good) {
  for (auto g : good) {
    DYNAMIC_SECTION(g.name) {
      auto in = std::istringstream{g.input};
      auto out = std::ostringstream{};

      stiX::macro_process(in, out);

      REQUIRE(g.expected == out.str());
    }
  }
}

TEST_CASE("Text replacement") {
  auto good = std::vector<good_case> {
    { "pass through", "nothing going on here\nat all", "nothing going on here\nat all" },
    { "simple define", "nothing going on here\ndefine(x, y)\nat all", "nothing going on here\n\nat all"},
    { "define with parenthesised replacement",
      "nothing going on here\ndefine(ENDFILE, (-1))\nat all",
      "nothing going on here\n\nat all" },
    { "simple replacement",
      "define(DONE, ENDFILE)\nif (getit(line) = DONE) then putit(sumline)",
      "\nif (getit(line) = ENDFILE) then putit(sumline)" },
    { "nested replacement",
      "define(ENDFILE, (-1))\ndefine(DONE, ENDFILE)\nif (getit(line) = DONE) then putit(sumline)",
      "\n\nif (getit(line) = (-1)) then putit(sumline)" },
    { "nested replacement, opposite order",
      "define(DONE, ENDFILE)\ndefine(ENDFILE, (-1))\nif (getit(line) = DONE) then putit(sumline)",
      "\n\nif (getit(line) = (-1)) then putit(sumline)" },
    { "just a define", "define(one, two)", "" },
    { "looks like an argument, but isn't", "define(dollar, $)\ndollar!", "\n$!"},
    { "expansion includes $", "define(dollar, $$1)\ndollar(5)!", "\n$5!"}
  };
  build_good_tests(good);
}

TEST_CASE("Text replacement with arguments") {
  auto good = std::vector<good_case> {
    { "arguments provided, not used",
      "define(x, y)x x(q) (x)",
      "y y (y)" },
    { "one arg, used",
      "define(def, int $1;) def(x)",
      " int x;" },
    { "one arg, used twice",
      "define(incr, $1:=$1+1)\nincr(x)\nincr(y)",
      "\nx:=x+1\ny:=y+1" },
    { "two args",
      "define(smash, $1$2)\nsmash(monkey, trousers)",
      "\nmonkeytrousers" },
    { "two args, only one given",
        "define(smash, $1$2)\nsmash(monkey)",
        "\nmonkey" },
    { "two args, with a macro as an argument",
      "define(ENDFILE, (-1))\ndefine(smash, $1$2)\nsmash(monkey, ENDFILE)",
      "\n\nmonkey(-1)" },
    { "three args, only one given",
      "define(smash, $1!$2?$3#)\nsmash(monkey)",
      "\nmonkey!?#" },
    { "three args, two given",
      "define(smash, $1!$2?$3#)\nsmash(monkey, trousers)",
      "\nmonkey!trousers?#" },
    { "three args, three given",
      "define(smash, $1!$2?$3#)\nsmash(hairy, monkey, trousers)",
      "\nhairy!monkey?trousers#" },
    { "three args, one empty",
      "define(smash, $1!$2?$3#)\nsmash(hairy, , trousers)",
      "\nhairy!?trousers#" },
    { "three args, two empty",
      "define(smash, $1!$2?$3#)\nsmash(,,trousers)",
      "\n!?trousers#" },
    { "three args, two empty",
      "define(smash, $1!$2?$3#)\nsmash(, monkey, )",
      "\n!monkey?#" },
    { "three args, all empty",
      "define(smash, $1!$2?$3#)\nsmash(,,)",
      "\n!?#" },
    { "two args, left recursive",
      "define(concat, $1-$2+)\nconcat(concat(A, B), C)",
      "\nA-B+-C+" },
    { "two args, right recursive",
      "define(concat, $1-$2+)\nconcat(A, concat(B, C))",
      "\nA-B-C++" },
    { "two args, recursive on both sides",
      "define(concat, $1-$2+)\nconcat(concat(A, B), concat(C, D))",
      "\nA-B+-C-D++" },
    { "two args, right recursive with a left recurse",
        "define(concat, $1-$2+)\nconcat(A, concat(concat(B, C), D))",
        "\nA-B-C+-D++" }
  };
  build_good_tests(good);
}

TEST_CASE("len()") {
  auto good = std::vector<good_case>{
    { "len of a single token",
      "len(string) is 6",
      "6 is 6" }
  };

  build_good_tests(good);
}

TEST_CASE("Ill-formed macros") {
  auto bad = std::vector<std::pair<std::string, std::string>> {
    { "define[x, y]", "Expected (" },
    { "define", "Expected (" },
    { "define(x:y)", "Expected ," },
    { "define(x, y]", "Expected )" },
    { "define(x, (((y))", "Expected )" },
    { "define(99, x)", "99 is not alphanumeric" },
    { "len(a string that just flops off the end", "Expected )" }
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