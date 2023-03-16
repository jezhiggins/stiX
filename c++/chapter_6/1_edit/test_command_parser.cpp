#define ADDITIONAL_TESTS

#include "command_parser.hpp"
#include "../../testlib/testlib.hpp"
#include "lines.hpp"

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

  using parse_test_cases = std::vector<parse_test_case>;

  auto good_indexes_test_cases = parse_test_cases {
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
    {"1+1+1+1=",              {"1+1+1+1=", 3, 5},   {4,  4,  '='}},
    {"1+2+3+4=",              {"1+2+3+4=", 3, 15},  {10, 10, '='}},
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
    {"-1,+1 when dot is 5",   {"-1,+1",    5, 10},  {4,  6,  '\n'}},
    {"1,2,3",                 {"1,2,3",    5, 10},  {2,  3,  '\n'}},
    {"1,2,3,",                {"1,2,3,",   5, 10},  {2,  3,  '\n'}},
    {"1,2,3,4",               {"1,2,3,4",  5, 10},  {3,  4,  '\n'}},
    {"1,2,3,4,",              {"1,2,3,4,", 5, 10},  {3,  4,  '\n'}},
    {"1,2,.",                 {"1,2,.",    5, 10},  {2,  5,  '\n'}}
  };

  auto bad_indexes_test_cases = parse_test_cases {
    {"no hex please",         {"1a,3=", 5, 10}},
    {"dot on rhs of -",       {"5-.",   5, 10}},
    {"dollar on rhs of -",    {"15-$",  5, 10}},
    {"begin>end",             {"5,4",   5, 10}},
    {"$-.",                   {"$-.",   5, 10}},
    {"from index too large",  { "99",   5, 10}},
    {"to index too large",    { "1,99", 5, 10}},
    {"arithmetic underflow",  { "-20",  5, 10}},
    {". arithmetic underflow",{ ".-20", 5, 10}},
    {"$ arithmetic underflow",{ "$-20", 5, 10}},
    {"arithmetic overflow",   { "+20",  5, 10}},
    {". arithmetic overflow", { ".+20", 5, 10}},
    {"$ arithmetic overflow", { "$+20", 5, 10}}
  };

  auto forward_search_test_cases = parse_test_cases {
    { "from 1, hits 1", { "/lines 1/", 1, 5}, { 1, 1, '\n'}}
  };
}

class buffer_double : public stiX::lines {
public:
  buffer_double(size_t d, size_t l):
    dot_(d), dollar_(l) { }

  size_t dot() const override { return dot_; }
  size_t last() const override { return dollar_; }

private:
  size_t const dot_;
  size_t const dollar_;
  std::vector<std::string> lines_ = {
    { "line 1" },
    { "line 2" },
    { "line 3" },
    { "line 4" },
    { "line 5" },
    { "line 6" },
    { "line 7" },
    { "line 8" },
    { "line 9" },
    { "line 10" },
  };
};

void index_test(parse_test_case tc, stiX::command expected) {
  SECTION(tc.label) {
    auto parsed_command = stiX::parse_command(
      tc.input.input
    );
    auto buffer = buffer_double(tc.input.dot,
                                tc.input.dollar);
    auto command = parsed_command.compile(buffer);

    REQUIRE(command == expected);
  }
}

void indexes_are_good(parse_test_case tc) {
  index_test(tc, tc.expected);
}

void indexes_are_bad(parse_test_case tc) {
  index_test(tc, stiX::command::error);
}

template <typename Function>
void all_tests(
  std::vector<parse_test_case> const& tests,
  Function test_fn
) {
  std::for_each(
    tests.begin(),
    tests.end(),
    test_fn
  );
}

TEST_CASE("Chapter 6 - edit - command parser") {
  SECTION("Good line indexes") {
    all_tests(good_indexes_test_cases, indexes_are_good);
  }

  SECTION("Bad line indexes") {
    all_tests(bad_indexes_test_cases, indexes_are_bad);
  }

  SECTION("Forward context search") {
    all_tests(forward_search_test_cases, indexes_are_good);
  }
}
