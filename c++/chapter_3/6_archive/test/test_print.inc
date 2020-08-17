#include "../print.hpp"

void test_print(
  std::string const& archive,
  std::vector<std::string> const& to_print,
  std::string const& expected
);

TEST_CASE("Chapter 3 - archive print mock") {
  SECTION("Empty archive") {
    test_print("", { }, "");
  }
  SECTION("Archive with contents, but print nothing") {
    test_print(
      archiveWithContents,
      { },
      ""
    );
  }
  SECTION("First file in archive") {
    test_print(
      archiveWithContents,
      { "testFile1" },
      testFile1
    );
  }
  SECTION("Second file in archive") {
    test_print(
      archiveWithContents,
      { "testFile2" },
      testFile2
    );
  }
  SECTION("Last file in archive") {
    test_print(
      archiveWithContents,
      { "testFile3" },
      testFile3
    );
  }
}

void test_print(
  std::string const& archive,
  std::vector<std::string> const& to_print,
  std::string const& expected
) {
  auto archive_in = std::istringstream(archive);
  auto out = std::ostringstream();

  stiX::print_files(archive_in, to_print, out);

  REQUIRE(out.str() == expected);
} // test_print