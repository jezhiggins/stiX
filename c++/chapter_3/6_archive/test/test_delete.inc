#include "../delete.hpp"

void test_delete(
  std::string const& archive,
  std::vector<std::string> const& to_remove,
  std::string const& expected
);

std::string const archiveWithContents =
  "-h- testFile1 17\nI am a test file\n-h- testFile2 14\nAnother file!\n";

TEST_CASE("Chapter 3 - archive delete mock") {
  SECTION("Empty archive") {
    test_delete(
      "",
      {"fruit"},
      ""
    );
  }
  SECTION("Remove nothing from the archive") {
    test_delete(
      archiveWithContents,
      { },
      archiveWithContents
    );
  }
}

void test_delete(
  std::string const& archive,
  std::vector<std::string> const& to_remove,
  std::string const& expected
) {
  auto archive_in = std::istringstream(archive);
  auto out = std::ostringstream();

  stiX::delete_from_archive(archive_in, to_remove, out);

  REQUIRE(out.str() == expected);
} // test_delete