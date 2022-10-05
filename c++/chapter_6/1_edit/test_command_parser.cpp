#define ADDITIONAL_TESTS
#include "../../testlib/testlib.hpp"

#include "command_parser.hpp"

namespace {
  struct parse_test_input {
    std::string input;
    size_t dot;
    size_t dollar;
  };

  struct parse_test_expected {
    size_t from;
    size_t to;
    char code;
  };

  struct parse_test_case {
    std::string label;
    parse_test_input input;
    parse_test_expected expected;
  };

  auto test_cases = std::vector<parse_test_case>{
    {"no input,empty buffer", {"",     0, 0},   {0,  0,  '\n'}},
    {"no input, dot is set",  {"",     3, 3},   {3,  3,  '\n'}},
    {"single letter",         {"i",    3, 5},   {3,  3,  'i'}},
    {"1p",                    {"1p",   3, 5},   {1,  1,  'p'}},
    {"99p",                   {"99p",  3, 500}, {99, 99, 'p'}},
    {".p",                    {".p",   3, 5},   {3,  3,  'p'}},
    {"$p",                    {"$p",   3, 5},   {5,  5,  'p'}},
    {"1,$p",                  {"1,$p", 3, 5},   {1,  5,  'p'}},
    {".,4p",                  {".,4p", 3, 5},   {3,  4,  'p'}},
    {"1,.p",                  {"1,.p", 3, 5},   {1,  3,  'p'}},
    {".,$",                   {".,$",  3, 5},   {3,  5,  '\n'}}
  };
}

TEST_CASE("Chapter 6 - edit - command parser") {
  std::for_each(
    test_cases.begin(),
    test_cases.end(),
    [](auto tc) {
      SECTION(tc.label) {
        auto command = stiX::parse_command(
          tc.input.input,
          tc.input.dot,
          tc.input.dollar
        );

        REQUIRE(command.from_index == tc.expected.from);
        REQUIRE(command.to_index == tc.expected.to);
        REQUIRE(command.code == tc.expected.code);
      }
    }
  );
}
