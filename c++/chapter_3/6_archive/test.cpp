#include "../../testlib/testlib.h"
#include "create.hpp"

std::string testFile1 = "I am a test file\n";
std::string testFile2 = "I am another file\n";

std::map<std::string, std::string> testFiles = {
    { "testFile1", testFile1 },
    { "testFile2", testFile2 }
};

std::istringstream testFileContents(std::string const& name) {
  auto tf = testFiles.find(name);
  if (tf == testFiles.end())
    return std::istringstream();
  return std::istringstream(tf->second);
} // testFileContents

TEST_CASE("Chapter 3 - archive create") {
  struct createArchiveTest {
    std::string const title;
    std::vector<stiX::input_file> const inputs;
    std::string const expected;
  };

  createArchiveTest tests[] = {
    {
      "no input files creates empty archive",
      { },
      ""
    },
    {
      "one zero-length input file",
      { { "nothing", 0 } },
      "-h- nothing 0\n"
    },
    {
      "two zero-length input files",
      {
        {"nothing", 0},
        {"empty", 0}
      },
      "-h- nothing 0\n-h- empty 0\n"
    },
    {
      "one file, with contents",
      { { "testFile1", testFile1.size() } },
      "-h- testFile1 " + std::to_string(testFile1.size()) + "\n" + testFile1
    },
    {
      "two files, with contents",
      {
        { "testFile1", testFile1.size() },
        { "testFile2", testFile2.size() }
      },
      "-h- testFile1 " + std::to_string(testFile1.size()) + "\n" + testFile1 +
      "-h- testFile2 " + std::to_string(testFile2.size()) + "\n" + testFile2
    }
  };

  for (auto t : tests) {
    DYNAMIC_SECTION(t.title) {
      std::ostringstream archiveOut;

      create_archive(t.inputs, archiveOut, testFileContents);

      auto archive = archiveOut.str();
      REQUIRE(archive == t.expected);
    }
  }
}