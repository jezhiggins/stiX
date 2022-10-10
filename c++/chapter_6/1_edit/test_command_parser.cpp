#define ADDITIONAL_TESTS

#include <iostream>
#include "command_parser.hpp"

std::ostream& operator<<(std::ostream& os, stiX::command const& c) {
  os << "{ " << c.from_index << ", " << c.to_index << ", " << c.code << " }";
  return os;
}

#include "../../testlib/testlib.hpp"


namespace {
  struct parse_test_input {
    std::string input;
    size_t dot;
    size_t dollar;
  };

  struct parse_test_case {
    std::string label;
    parse_test_input input;
    stiX::command expected;
  };

  auto good_test_cases = std::vector<parse_test_case>{
    {"no input,empty buffer", {"",         0, 0},   {0,  0,  '\n'}},
    {"no input, dot is set",  {"",         3, 3},   {3,  3,  '\n'}},
    {"single letter",         {"i",        3, 5},   {3,  3,  'i'}},
    {"lone digit",            {"1",        3, 5},   {1,  1,  '\n'}},
    {".",                     {".",        3, 5},   {3,  3,  '\n'}},
    {"$",                     {"$",        3, 5},   {5,  5,  '\n'}},
    {"1p",                    {"1p",       3, 5},   {1,  1,  'p'}},
    {"99p",                   {"99p",      3, 500}, {99, 99, 'p'}},
    {".p",                    {".p",       3, 5},   {3,  3,  'p'}},
    {"$p",                    {"$p",       3, 5},   {5,  5,  'p'}},
    {"1,$p",                  {"1,$p",     3, 5},   {1,  5,  'p'}},
    {"1,p",                   {"1,p",      3, 5},   {1,  1,  'p'}},
    {"1;$p",                  {"1;$p",     3, 5},   {1,  5,  'p'}},
    {"1;p",                   {"1;p",      3, 5},   {1,  1,  'p'}},
    {".,4p",                  {".,4p",     3, 5},   {3,  4,  'p'}},
    {"1,.p",                  {"1,.p",     3, 5},   {1,  3,  'p'}},
    {".,$",                   {".,$",      3, 5},   {3,  5,  '\n'}},
    {"1+1=",                  {"1+1=",     3, 5},   {2,  2,  '='}},
    {"4-1=",                  {"4-1=",     3, 5},   {3,  3,  '='}},
    {".+1=",                  {".+1=",     3, 5},   {4,  4,  '='}},
    {".-2=",                  {".-2=",     3, 5},   {1,  1,  '='}},
    {"$-4=",                  {"$-1=",     3, 6},   {5,  5,  '='}},
    {".+5,$-6p",              {".+5,$-6p", 5, 20},  {10, 14, 'p'}},
    {"-1 when dot is 5",      {"-1",       5, 10},  {4,  4,  '\n'}},
    {"-2 when dot is 5",      {"-2",       5, 10},  {3,  3,  '\n'}},
    {"1,-2 when dot is 5",    {"1,-2",     5, 10},  {1,  3,  '\n'}},
    {".,+1 when dot is 5",    {".,+1",     5, 10},  {5,  6,  '\n'}},
    {"-1,. when dot is 5",    {"-1,.",     5, 10},  {4,  5,  '\n'}},
    {"-1,+1 when dot is 5",   {"-1,+1",    5, 10},  {4,  6,  '\n'}}
  };

  auto bad_test_cases = std::vector<parse_test_case>{
    {"no hex please",   {"1a,3=", 5, 10}},
    {"5-.",             {"5-.", 5, 10}},
    {"15-$",            {"15-$", 5, 10}},
    {"$-.",             {"$-.", 5, 10}},
  };
}

TEST_CASE("Chapter 6 - edit - command parser") {
  SECTION("Good line indexes") {
    std::for_each(
      good_test_cases.begin(),
      good_test_cases.end(),
      [](auto tc) {
        SECTION(tc.label) {
          auto command = stiX::parse_command(
            tc.input.input,
            tc.input.dot,
            tc.input.dollar
          );

          REQUIRE(command.from_index == tc.expected.from_index);
          REQUIRE(command.to_index == tc.expected.to_index);
          REQUIRE(command.code == tc.expected.code);
        }
      }
    );
  }

  SECTION("Bad line indexes") {
    std::for_each(
      bad_test_cases.begin(),
      bad_test_cases.end(),
      [](auto tc) {
        SECTION(tc.label) {
          auto command = stiX::parse_command(
            tc.input.input,
            tc.input.dot,
            tc.input.dollar
          );

          REQUIRE(command == tc.expected);
        }
      }
    );
  }
}
