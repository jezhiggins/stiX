#include <filesystem>
#include "../../testlib/testlib.h"
#include "../../lib/file_open.hpp"

namespace fs = std::filesystem;

std::string asArchive(std::string const& name, std::string const& contents) {
  auto o = std::ostringstream();

  o << "-h- "
    << name
    << " "
    << contents.size()
    << "\n"
    << contents;

  return o.str();
}

std::string testFile1 = "I am a test file\n";
std::string testFile2 = "I am another file\n";
std::string testFile3 = "Yet another file!\n";

auto const archiveTestFile1 = asArchive("testFile1", testFile1);
auto const archiveTestFile2 = asArchive("testFile2", testFile2);
auto const archiveTestFile3 = asArchive("testFile3", testFile3);

auto const archiveWithContents =
  archiveTestFile1 + archiveTestFile2 + archiveTestFile3;

class working_directory {
public:
  working_directory(std::string const& fixture) :
    pwd_(fs::current_path()),
    cwd_(fs::current_path() / "fixture" / fixture) {
    fs::current_path(cwd_);
  } // FileTestFixture

  operator fs::path() const {
    return cwd_;
  } // operation fs::path

  ~working_directory() {
    fs::current_path(pwd_);
  } // ~FileTestFixture

private:
  fs::path pwd_;
  fs::path cwd_;
}; // working_directory

#include "test/test_create.inc"
#include "test/test_table.inc"
#include "test/test_delete.inc"
#include "test/test_print.inc"