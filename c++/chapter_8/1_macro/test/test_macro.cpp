#include "../../../testlib/testlib.hpp"
#include "../macro.hpp"
#include "test.hpp"

TEST_CASE("Passthrough") {
  auto good = std::vector<good_case> {
    { "pass through", "nothing going on here\nat all", "nothing going on here\nat all" },
    { "the word define is not special by itself", "the word define is not special by itself", "the word define is not special by itself" },
    { "define[x, y]", "define[x, y]", "define[x, y]" },
    { "define", "define", "define" }
  };
  build_good_tests(good);
}

TEST_CASE("Text replacement") {
  auto good = std::vector<good_case> {
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
    { "replacement in definition",
      "define(DONE, ENDFILE)\ndefine(DONE, (-1))\nif (getit(line) = ENDFILE) then putit(sumline)",
      "\n\nif (getit(line) = (-1)) then putit(sumline)" },
    { "just a define", "define(one, two)", "" },
    { "looks like an argument, but isn't", "define(dollar, $)\ndollar!", "\n$!"},
    { "expansion includes $", "define(dollar, $$1)\ndollar(5)!", "\n$5!"},
    { "replace includes $1", "define(parp, hello$1world)parp parp(-)", "helloworld hello-world" },
    { "comma but no replacement", "define(parp,)parp", "" },
    { "no replacement", "define(parp)-parp-parp", "--"}
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

TEST_CASE("effect of quotes") {
  auto good = std::vector<good_case> {
    { "len($1) in replacement",
        "define(howlong, len($1)) howlong howlong(fruit)",
        " 2 2" },
    { "`len($1)' in replacement",
        "define(howlong, `len($1)') howlong howlong(fruit)",
        " 0 5" },
    { "len`($1)' in replacement",
      "define(howlong, len`($1)') howlong howlong(fruit)",
      " 0 5" },
    { "len(`$'1) in replacement",
      "define(howlong, len(`$'1)) howlong howlong(fruit)",
      " 2 2" },
    { "len(`$1') in replacement",
      "define(howlong, len(`$1')) howlong howlong(fruit)",
      " 2 2" },
    { "create an alias for define, use it",
        "define(def, `define($1,$2)') def(fish, fowl) fish",
        "  fowl" },
    { "whole expression is quoted",
        "`define($1,$2)'!",
        "define($1,$2)!" }
  };
  build_good_tests(good);
}

TEST_CASE("Ill-formed macros causing errors") {
  auto bad = std::vector<bad_case> {
    { "define(x, y]", "Expected )" },
    { "define(x, (((y))", "Expected )" },
  };
  build_bad_tests(bad);
}

TEST_CASE("Valid but meaningless") {
  auto meaningless = std::vector<good_case> {
    { "x:y is a multitoken sequence", "define(x:y, replacement) x:y", " x:y" },
    { "99 is not alphanumeric", "define(99, x) 99", " 99" },
    { "empty define", "define() hello", " hello" },
    { "empty name", "define(, replace) hello", " hello" }
  };
  build_good_tests(meaningless);
}

TEST_CASE("Valid with warning") {
  auto warnings = std::vector<warning_case> {
    { "too many args to define", "define(x,y,z) x", " y", "Warning: excess arguments to `define' ignored\n" },
  };
  build_warning_tests(warnings);
}
