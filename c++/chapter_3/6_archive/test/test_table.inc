#include "../table.hpp"

void test_table(
  std::string const& archive,
  std::string const& expected
) {
  auto archive_in = std::istringstream(archive);
  auto out = std::ostringstream();

  stiX::table_archive(archive_in, out);

  REQUIRE(out.str() == expected);
} // test_table

TEST_CASE("Chapter 3 - archive table") {
  SECTION("Empty archive") {
    test_table("", "");
  }

  SECTION("One zero length file") {
    test_table(
      "-h- nothing 0\n",
      "nothing\t0\n"
    );
  }

  SECTION("Two zero length files") {
    test_table(
      "-h- nothing 0\n-h- empty 0\n",
      "nothing\t0\nempty\t0\n"
    );
  }

  SECTION("Two files with contents") {
    test_table(
      "-h- testFile1 17\nI am a test file\n-h- testFile2 14\nAnother file!\n",
      "testFile1\t17\ntestFile2\t14\n"
    );
  }
}