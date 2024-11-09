#ifndef STICPP_TEST_HPP
#define STICPP_TEST_HPP

#include <string>
#include <vector>

struct good_case {
  std::string name;
  std::string input;
  std::string expected;
};

struct warning_case {
  std::string name;
  std::string input;
  std::string output;
  std::string warning;
};

struct bad_case {
  std::string input;
  std::string exception;
};

void build_good_tests(std::vector<good_case> const& good);
void build_bad_tests(std::vector<bad_case> const& bad);
void build_warning_tests(std::vector<warning_case> const& warnings);

#endif //STICPP_TEST_HPP
