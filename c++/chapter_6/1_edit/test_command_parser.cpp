#define ADDITIONAL_TESTS

#include "command_parser.hpp"
#include "../../testlib/testlib.hpp"
#include "lines.hpp"

using namespace std::string_literals;

namespace {
  struct parse_test_input {
    std::string input;
    size_t dot;
    size_t dollar;
  };

  struct parse_test_extras {
    std::string filename;
    size_t destination;
    std::string search_pattern;
    std::string replacement;
  };

  struct parse_test_expectation {
    size_t from;
    size_t to;
    size_t dot;
    char code;
    parse_test_extras extras;
  };

  struct parse_test_case {
    std::string label;
    parse_test_input input;
    parse_test_expectation expected;
  };

  using parse_test_cases = std::vector<parse_test_case>;

  auto good_indexes_test_cases = parse_test_cases {
    {"no input, dot is set",  {"",         1, 3},   {1,  2,  2, '\n'}},
    {"single letter",         {"i",        3, 5},   {3,  3,  3, 'i'}},
    {"lone digit",            {"1",        3, 5},   {1,  1,  3, '\n'}},
    {".",                     {".",        3, 5},   {3,  3,  3, '\n'}},
    {"$",                     {"$",        3, 5},   {5,  5,  3, '\n'}},
    {"1p",                    {"1p",       3, 5},   {1,  1,  3, 'p'}},
    {"99p",                   {"99p",      3, 500}, {99, 99, 3, 'p'}},
    {".p",                    {".p",       3, 5},   {3,  3,  3, 'p'}},
    {"$p",                    {"$p",       3, 5},   {5,  5,  3, 'p'}},
    {"1,$p",                  {"1,$p",     3, 5},   {1,  5,  3, 'p'}},
    {"1,p",                   {"1,p",      3, 5},   {1,  1,  3, 'p'}},
    {"1;$p",                  {"1;$p",     3, 5},   {1,  5,  1, 'p'}},
    {"1;p",                   {"1;p",      3, 5},   {1,  1,  1, 'p'}},
    {".,4p",                  {".,4p",     3, 5},   {3,  4,  3, 'p'}},
    {"1,.p",                  {"1,.p",     3, 5},   {1,  3,  3, 'p'}},
    {".,$",                   {".,$",      3, 5},   {3,  5,  3, '\n'}},
    {"1+1=",                  {"1+1=",     3, 5},   {2,  2,  3, '='}},
    {"1+1+1+1=",              {"1+1+1+1=", 3, 5},   {4,  4,  3, '='}},
    {"1+2+3+4=",              {"1+2+3+4=", 3, 15},  {10, 10, 3, '='}},
    {"4-1=",                  {"4-1=",     3, 5},   {3,  3,  3, '='}},
    {".+1=",                  {".+1=",     3, 5},   {4,  4,  3, '='}},
    {".-2=",                  {".-2=",     3, 5},   {1,  1,  3, '='}},
    {"$-4=",                  {"$-1=",     3, 6},   {5,  5,  3, '='}},
    {".+5,$-6p",              {".+5,$-6p", 5, 20},  {10, 14, 5, 'p'}},
    {"-1 when dot is 5",      {"-1",       5, 10},  {4,  4,  5, '\n'}},
    {"-2 when dot is 5",      {"-2",       5, 10},  {3,  3,  5, '\n'}},
    {"1,-2 when dot is 5",    {"1,-2",     5, 10},  {1,  3,  5, '\n'}},
    {".,+1 when dot is 5",    {".,+1",     5, 10},  {5,  6,  5, '\n'}},
    {"-1,. when dot is 5",    {"-1,.",     5, 10},  {4,  5,  5, '\n'}},
    {"-1,+1 when dot is 5",   {"-1,+1",    5, 10},  {4,  6,  5, '\n'}},
    {"1,2,3",                 {"1,2,3",    5, 10},  {2,  3,  5, '\n'}},
    {"1,2,3,",                {"1,2,3,",   5, 10},  {2,  3,  5, '\n'}},
    {"1,2,3,4",               {"1,2,3,4",  5, 10},  {3,  4,  5, '\n'}},
    {"1,2,3,4,",              {"1,2,3,4,", 5, 10},  {3,  4,  5, '\n'}},
    {".+1,.+1,.+1,.+1,",      {".+1,.+1,.+1,.+1,", 5, 10},  {6,  6, 5, '\n'}},
    {".+1;.+1;.+1;.+1;",      {".+1;.+1;.+1;.+1;", 5, 10},  {8,  9, 9, '\n'}},
    {"1,2,.",                 {"1,2,.",    5, 10},  {2,  5,  5, '\n'}},
    {"1,3m 6",                {"1,3m 6",   5, 10},  {1,  3,  5, 'm', { .destination = 6}}},
    {"1,3m .",                {"1,3m .",   5, 10},  {1,  3,  5, 'm', { .destination = 5}}},
    {"1,3m .+3",              {"1,3m .+3", 5, 10},  {1,  3,  5, 'm', { .destination = 8}}},
    {"1,3;m .+4",             {"1,3;m .+4",5, 10},  {1,  3,  3, 'm', { .destination = 7}}},
    {"1,3m6",                 {"1,3m6",    5, 10},  {1,  3,  5, 'm', { .destination = 6}}},
    {"1,3m.",                 {"1,3m.",    5, 10},  {1,  3,  5, 'm', { .destination = 5}}},
    {"1,3m.+3",               {"1,3m.+3",  5, 10},  {1,  3,  5, 'm', { .destination = 8}}},
    {"1,3;m.+4",              {"1,3;m.+4", 5, 10},  {1,  3,  3, 'm', { .destination = 7}}},
    {"e file",                {"e f.txt",  0, 0},   {0,  0,  0, 'e', { .filename = "f.txt"}}},
    {"e space, no filename",  {"e ",       0, 0},   {0,  0,  0, 'e'}},
    {"e spaces before name",  {"e  f.txt", 0, 0},   {0,  0,  0, 'e', { .filename = "f.txt"}}},
    {"e no filename",         {"e",        0, 0},   {0,  0,  0, 'e'}},
    {"1,3w f.c",              {"1,3w f.c", 5, 10},  {1,  3,  5, 'w', { .filename = "f.c" }}},
    {"1,3w",                  {"1,3w",     5, 10},  {1,  3,  5, 'w'}},
    {"w f.c",                 {"w f.c",    5, 10},  {1,  10, 5, 'w', { .filename = "f.c" }}},
    {"w",                     {"w",        5, 10},  {1,  10, 5, 'w'}},
    {"quit empty buffer",     {"q",        0, 0},   {0,  0,  0, 'q'}},
    {"quit",                  {"q",        5, 10},  {5,  5,  5, 'q'}},
    {"1,2,3,4\\n",            {"1,2,3,4\n",5, 10},  {3,  4,  5, '\n'}},
    {"7\\n",                  {"7\n",      5, 10},  {7,  7,  5, '\n'}},
    {"\\n",                   {"\n",       5, 10},  {5,  6,  6, '\n'}},
    { "1,$s/fruit/veg/",      {"1,$s/fruit/veg/", 5, 10},   {1, 10, 5, 's', { .search_pattern = "fruit", .replacement = "veg" }}}
  };

  auto bad_indexes_test_cases = parse_test_cases {
    {"no input,empty buffer", {"",      0, 0}},
    {"no input, dot is $",    {"",      3, 3}},
    {"no hex please",         {"1a,3=", 5, 10}},
    {"dot on rhs of -",       {"5-.",   5, 10}},
    {"dollar on rhs of -",    {"15-$",  5, 10}},
    {"begin>end",             {"5,4",   5, 10}},
    {"$-.",                   {"$-.",   5, 10}},
    {"from index too large",  {"99",   5, 10}},
    {"to index too large",    {"1,99", 5, 10}},
    {"arithmetic underflow",  {"-20",  5, 10}},
    {". arithmetic underflow",{".-20", 5, 10}},
    {"$ arithmetic underflow",{"$-20", 5, 10}},
    {"arithmetic overflow",   {"+20",  5, 10}},
    {". arithmetic overflow", {".+20", 5, 10}},
    {"$ arithmetic overflow", {"$+20", 5, 10}},
    {"e with line numbers",   {"1,2e file", 5, 10}},
    {"e trailing spaces",     {"e f.txt ", 0, 0}},
    {"e no space before name",  {"ef.txt",  0, 0}},
    {"q with line numbers",     {"1,2q",    5, 10}},
    {"m no destination",        {"1,2m",    5, 10}},
    {"m destination overflow",  {"1,2m 55", 5, 10}},
    {"m destination underflow", {"1,2m .-10", 5, 10}},
    {"m destination overlap  ", {"1,5m3",     5, 10}},
    {"1,$s",                    {"1,$s",      5, 10}},
    {"1,$s/w/",                 {"1,$s/w/",   5, 10}},
    {"1,$s/w/W/?",              {"1,$s/w/W/?",5, 10}},
    {"s",                       {"s",         5, 10}},
    {"s/w/",                    {"s/w/",      5, 10}},
    {"s/w/W",                   {"s/w/W",     5, 10}},
    {"s/w/W/?",                 {"s/w/W/?",   5, 10}}
  };

  auto forward_search_tests = parse_test_cases {
    {"from 1, hits 2",  {"/line 2/", 1, 5},  {2, 2, 1, '\n'}},
    {"from 1, hits 10", {"/line 1/", 1, 10}, {10, 10, 1, '\n'}},
    {"from 1, hits 1",  {"/line 1/", 1, 5},  {1, 1, 1, '\n'}},
    {"from 5, hits 10", {"/line 1/", 5, 10}, {10, 10, 5, '\n'}},
    {"from 5, hits 3",  {"/line 3/", 5, 10}, {3, 3, 5, '\n'}},
    {"search with arithmetic",{"/line 9/-2", 5, 10}, {7, 7, 5, '\n'}}
  };
  auto bad_forward_search_tests = parse_test_cases {
    {"pattern doesn't match", {"/fruit/",    1, 5}},
    {"match after const",     {"/line 4/,2", 1, 5}}
  };

  auto backward_search_tests = parse_test_cases {
    {"from 1, hits 2",  {"\\line 2\\", 1, 5},  {2, 2, 1, '\n'}},
    {"from 1, hits 10", {"\\line 1\\", 1, 10}, {10, 10, 1, '\n'}},
    {"from 1, hits 1",  {"\\line 1\\", 1, 5},  {1, 1, 1, '\n'}},
    {"from 5, hits 9",  {"\\line 9\\", 5, 10}, {9, 9, 5, '\n'}},
    {"from 5, hits 3",  {"\\line 3\\", 5, 10}, {3, 3, 5, '\n'}},
    {"search with arithmetic",{"\\line 9\\-2", 5, 10}, {7, 7, 5, '\n'}},
  };
  auto bad_backward_search_tests = parse_test_cases {
    {"pattern doesn't match", {"\\fruit\\",    1, 5}},
    {"match after const",     {"\\line 4\\,2", 1, 5}}
  };
}

namespace stiX {
  std::ostream &operator<<(std::ostream &out, command const &cmd) {
    out << "{ "
        << cmd.from_index
        << "-"
        << cmd.to_index
        << ", "
        << cmd.dot
        << ", '";
    if (cmd.code != '\n')
      out << cmd.code;
    else
      out << "\\n"s;
    out << "'";
    if (!cmd.filename.empty())
      out << ", " << cmd.filename;
    if (cmd.destination != std::string::npos)
      out << ", " << cmd.destination;
    out << " }";
    return out;
  }
}

class buffer_double : public stiX::lines {
public:
  buffer_double(size_t d, size_t l):
    dot_(d), dollar_(l) {
    lines_.emplace_back("underflow");
    for (auto i = 1; i <= dollar_; ++i)
      lines_.emplace_back("line " + std::to_string(i));
    lines_.emplace_back("overflow");
  }

  std::string_view line_at(size_t index) const override {
    return lines_[index];
  }

  size_t dot() const override { return dot_; }
  size_t last() const override { return dollar_; }

private:
  size_t const dot_;
  size_t const dollar_;
  std::vector<std::string> lines_;
};

void verify_from_to_dot_expectations(stiX::command command, parse_test_expectation expected) {
  REQUIRE(command.from_index == expected.from);
  REQUIRE(command.to_index == expected.to);
  REQUIRE(command.dot == expected.dot);
  REQUIRE(command.code == expected.code);
}

void verify_extras(stiX::parsed_command parsed_command, parse_test_expectation expected) {
  REQUIRE(parsed_command.extras.filename == expected.extras.filename);
  REQUIRE(parsed_command.extras.search_pattern == expected.extras.search_pattern);
  REQUIRE(parsed_command.extras.replacement == expected.extras.replacement);
}

////////////////////////////////////////////////
void indexes_are_good(parse_test_case const& tc) {
  SECTION(tc.label) {
    auto parsed_command = stiX::parse_command(
      tc.input.input
    );
    auto buffer = buffer_double(tc.input.dot,
                                tc.input.dollar);
    auto command = parsed_command.compile(buffer);

    verify_from_to_dot_expectations(command, tc.expected);

    if (parsed_command.extras.destination_expression != nullptr) {
      auto destination = parsed_command.extras.destination_expression(buffer, command.dot);
      INFO("destination");
      REQUIRE(destination == tc.expected.extras.destination);
    }
    verify_extras(parsed_command, tc.expected);
  }
}

void indexes_are_bad(parse_test_case const& tc) {
  SECTION(tc.label) {
    auto parsed_command = stiX::parse_command(
      tc.input.input
    );
    auto buffer = buffer_double(tc.input.dot,
                                tc.input.dollar);
    auto command = parsed_command.compile(buffer);

    REQUIRE(command.from_index == stiX::command::line_error);
    REQUIRE(command.to_index == stiX::command::line_error);
    REQUIRE(command.dot == stiX::command::line_error);
    REQUIRE(command.code == stiX::command::code_error);
  }
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
    all_tests(forward_search_tests, indexes_are_good);
    all_tests(bad_forward_search_tests, indexes_are_bad);
  }
  SECTION("Backward context search") {
    all_tests(backward_search_tests, indexes_are_good);
    all_tests(bad_backward_search_tests, indexes_are_bad);
  }
}
