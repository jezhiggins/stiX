#include "../../testlib/testlib.hpp"

#include "formatter.hpp"

#include <filesystem>
namespace fs = std::filesystem;

void format_test(
  std::istream& input,
  std::string const& expected);
std::string read_expected(fs::path test_file);

void test_fixture(fs::path const& test_file) {
  DYNAMIC_SECTION(test_file.filename()) {
    auto input = std::ifstream(test_file);
    format_test(input, read_expected(test_file));
  }
}
void find_tests_in_directory(fs::path const& dir) {
  DYNAMIC_SECTION(dir.filename()) {
    for(auto const& test_file : fs::directory_iterator { dir }) {
      if (test_file.path().extension() != ".txt")
        continue;

      test_fixture(test_file.path());
    }
  }
}

TEST_CASE("Test fixtures") {
  auto test_dir = fs::current_path() / "../chapter_7/1_format/test_fixtures";
  for (auto const& dir : fs::directory_iterator { test_dir }) {
    find_tests_in_directory(dir.path());
  }
}

void format_test(
    std::istream& input,
    std::string const& expected) {
  auto out = std::ostringstream { };

  auto formatter = stiX::screen_formatter { input, out };
  formatter.format();

  auto output = out.str();
  REQUIRE(output == expected);
}

std::string read_expected(fs::path test_file) {
  test_file.replace_extension(".exp");
  auto exp_file = std::ifstream(test_file);

  auto expected = std::string { };
  std::copy(
    std::istreambuf_iterator<char>{ exp_file },
    std::istreambuf_iterator<char>{ },
    std::back_inserter(expected)
  );
  return expected;
}

