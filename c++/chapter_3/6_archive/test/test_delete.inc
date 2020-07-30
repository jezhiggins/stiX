#include "../delete.hpp"

void test_delete(
  std::string const& archive,
  std::vector<std::string> const& to_remove,
  std::string const& expected
);

std::string const archiveTestFile1 = "-h- testFile1 17\nI am a test file\n";
std::string const archiveTestFile2 = "-h- testFile2 14\nAnother file!\n";
std::string const archiveTestFile3 = "-h- testFile3 18\nYet another file!\n";

std::string const archiveWithContents =
  archiveTestFile1 + archiveTestFile2 + archiveTestFile3;


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
  SECTION("Remove first file in three file archive") {
    test_delete(
      archiveWithContents,
      { "testFile1" },
      archiveTestFile2 + archiveTestFile3
    );
  }
  SECTION("Remove second file in three file archive") {
    test_delete(
      archiveWithContents,
      { "testFile2" },
      archiveTestFile1 + archiveTestFile3
    );
  }
  SECTION("Remove third file in three file archive") {
    test_delete(
      archiveWithContents,
      { "testFile3" },
      archiveTestFile1 + archiveTestFile2
    );
  }
  SECTION("Remove first and second file in three file archive") {
    test_delete(
      archiveWithContents,
      { "testFile1", "testFile2" },
      archiveTestFile3
    );
  }
  SECTION("Remove first and third file in three file archive") {
    test_delete(
      archiveWithContents,
      { "testFile1", "testFile3" },
      archiveTestFile2
    );
  }
  SECTION("Remove second and third file in three file archive") {
    test_delete(
      archiveWithContents,
      { "testFile3", "testFile2" },
      archiveTestFile1
    );
  }
  SECTION("Remove all files in three file archive") {
    test_delete(
      archiveWithContents,
      { "testFile1", "testFile2", "testFile3" },
      ""
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