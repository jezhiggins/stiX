-h- archive_file.cpp 1389
#include "archive_file.hpp"
#include <iostream>
#include <stdexcept>
#include <algorithm>

std::string const HEADER_TAG = "-h- ";

stiX::archive_file stiX::parse_header(std::string const& header) {
  if (header.find(HEADER_TAG) != 0)
    throw std::logic_error("Not a header line");

  auto lastspace = header.find_last_of(' ');
  auto namelength = lastspace-4;
  auto name = header.substr(4, namelength);
  auto size = header.substr(lastspace+1);

  return { name, std::stoul(size) };
} // parse_header

std::ostream& operator<<(std::ostream& os, stiX::archive_file const& af) {
  os << HEADER_TAG
     << af.name
     << ' '
     << af.filesize
     << '\n';
  return os;
} // operator<<

void stiX::skip_entry(
  std::istream& archive_in,
  stiX::archive_file const& header
) {
  archive_in.seekg(header.filesize, std::ios_base::cur);
} // skip_entry

bool stiX::of_interest(
  std::vector<std::string> const& files_to_remove,
  stiX::archive_file const& name
) {
  return std::find(files_to_remove.begin(), files_to_remove.end(), name.name) != files_to_remove.end();
} // of_interest

void stiX::copy_contents(
  std::istream& archive_in,
  stiX::archive_file const& header,
  std::ostream& archive_out
) {
  std::copy_n(
    std::istreambuf_iterator<char>(archive_in),
    header.filesize,
    std::ostreambuf_iterator<char>(archive_out)
  );

  archive_in.get();
} // copy_contents

-h- delete.hpp 306
#ifndef STICPP_DELETE_HPP
#define STICPP_DELETE_HPP

#include <iosfwd>
#include <vector>

namespace stiX {
  void delete_from_archive(
    std::istream& archive_in,
    std::vector<std::string> const& files_to_remove,
    std::ostream& archive_out
  );
} // delete_from_archive

#endif //STICPP_DELETE_HPP
-h- archive_file.hpp 1145
#ifndef STICPP_ARCHIVE_FILE_HPP
#define STICPP_ARCHIVE_FILE_HPP

#include <string>
#include <vector>
#include <iostream>
#include "../../lib/getline.hpp"

namespace stiX {
  struct archive_file {
    std::string name;
    size_t filesize;
  };

  archive_file parse_header(std::string const& header);

  void skip_entry(
    std::istream& archive_in,
    stiX::archive_file const& header
  );

  bool of_interest(
    std::vector<std::string> const& files_to_remove,
    stiX::archive_file const& name
  );

  void copy_contents(
    std::istream& archive_in,
    stiX::archive_file const& header,
    std::ostream& archive_out
  );

    template<typename ArchiveReader>
    void read_archive(
        std::istream& archive_in,
        ArchiveReader reader
    ) {
      archive_in.peek();

      while(archive_in && !archive_in.eof()) {
        auto header_line = getline(archive_in);
        auto header = parse_header(header_line);

        reader(archive_in, header);

        archive_in.peek();
      } // while ...
    } // read_archive
} // namespace stiX

std::ostream& operator<<(std::ostream& os, stiX::archive_file const& af);

#endif-h- delete.cpp 624
#include "delete.hpp"
#include <iostream>

#include "../../lib/getline.hpp"
#include "./archive_file.hpp"

void stiX::delete_from_archive(
  std::istream& archive_in,
  std::vector<std::string> const& files_to_remove,
  std::ostream& out
) {
  read_archive(
      archive_in,
      [&files_to_remove, &out](
        std::istream& archive_in,
        stiX::archive_file const& header
      ) {
        if (of_interest(files_to_remove, header))
          skip_entry(archive_in, header);
        else {
          out << header;
          copy_contents(archive_in, header, out);
        }
      }
  );
} // delete_from_archive

-h- test.cpp 5885
#include <filesystem>
#include "../../testlib/testlib.hpp"
#include "../../lib/file_open.hpp"
#include "../1_compare/compare.hpp"
#include "archive_file.hpp"
#include "archive.hpp"

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

std::vector<stiX::archive_file> list_input_files(fs::path const& directory) {
  auto files = std::vector<stiX::archive_file>();

  auto wd = working_directory(directory / "input");
  for(auto& p: fs::directory_iterator(wd)) {
    auto filepath = p.path();
    auto filename = fs::relative(filepath, wd).string();

    if (filename[0] == '.')
      continue;

    auto size = fs::file_size(filepath);

    files.emplace_back(stiX::archive_file { filename, size });
  }

  std::sort(files.begin(), files.end(),
            [](stiX::archive_file const& lhs, stiX::archive_file const& rhs) -> bool
            {
              return lhs.name < rhs.name;
            }
  );

  return files;
} // list_input_files

bool is_hidden_file(fs::path const& p) {
  return p.filename().string()[0] == '.';
} // is_hidden_file

void clean_output_directory(fs::path const& directory) {
  auto output_dir = directory / "output";

  for (auto& p: fs::directory_iterator(output_dir)) {
    if (is_hidden_file(p))
      continue;
    fs::remove(p);
  }
} // clean_output_directory

void setup_initial_files(fs::path const& directory) {
  auto initial_dir = directory / "initial";
  if (!fs::exists(initial_dir))
    return;

  auto output_dir = directory / "output";
  for (auto& p: fs::directory_iterator(initial_dir)) {
    if (is_hidden_file(p))
      continue;

    auto filename = p.path().filename();
    fs::copy(p, output_dir / filename);
  }
}

int count_files(fs::path const& dir) {
  return std::count_if(
      fs::directory_iterator(dir),
      fs::directory_iterator(),
      [](auto const& p) { return !is_hidden_file(p); }
  );
} // count_files

void matches_expected(fs::path const& name) {
  auto output_dir = name / "output";
  auto expected_dir = name / "expected";

  for(auto& p: fs::directory_iterator(output_dir)) {
    auto output_file = p.path();
    auto filename = output_file.filename();

    if (is_hidden_file(filename))
      continue;

    auto expected_file = expected_dir / filename;

    auto output_is = std::ifstream(output_file);
    auto expected_is = std::ifstream(expected_file);

    auto result = std::ostringstream();

    stiX::compare(
      output_file,
      output_is,
      expected_file,
      expected_is,
      result
    );


    INFO(result.str());
    REQUIRE(result.str().empty());
  }

  auto output_file_count = count_files(output_dir);
  auto expected_file_count = count_files(expected_dir);

  INFO("Expected output files")
  REQUIRE(output_file_count == expected_file_count);
} // matches_expected

fs::path output_file_name(fs::path const& directory) {
  return directory / "output" / "archive";
} // output_file_name

typedef std::function<void(
  std::vector<stiX::archive_file> const& input,
  std::ostream& archive_out
)> test_fn;

void file_test(
  std::string const& title,
  fs::path const& directory,
  test_fn fn
) {
  DYNAMIC_SECTION("Fixture " << title) {
    auto input_files = list_input_files(directory);

    {
      auto archive_out = std::ofstream(output_file_name(directory));
      auto wd = working_directory(directory / "input");
      fn(input_files, archive_out);
    }

    matches_expected(directory);
  }
} // file_test

void file_fixture(std::string const& name, test_fn fn) {
  auto fixture_dir = working_directory(name);

  for (auto& p : fs::directory_iterator(fixture_dir)) {
    auto title = fs::relative(p.path()).string();

    clean_output_directory(p);

    file_test(title, p.path(), fn);
  }
} // file_fixture

void app_file_test(
  std::string const& title,
  fs::path const& directory,
  std::string const& cmd,
  std::string const& archive_file_name
) {
  DYNAMIC_SECTION("App " << title) {
    auto input_files = list_input_files(directory);

    auto arguments = std::vector {cmd, archive_file_name };
    for (auto i : input_files)
      arguments.push_back(i.name);

    {
      auto wd = working_directory(directory / "input");
      stiX::archive(arguments);
    }

    matches_expected(directory);
  }
} // app_file_test


void app_fixture(std::string const& name, std::string const& cmd) {
  auto fixture_dir = working_directory(name);

  for (auto& p : fs::directory_iterator(fixture_dir)) {
    auto title = fs::relative(p.path()).string();

    clean_output_directory(p);
    setup_initial_files(p);

    auto output_file = output_file_name(p.path());
    app_file_test(title, p.path(), cmd, output_file);
  }
} // app_fixture

#include "test/test_create.inc"
#include "test/test_table.inc"
#include "test/test_delete.inc"
#include "test/test_print.inc"
#include "test/test_update.inc"-h- test/test_update.inc 66
TEST_CASE("Chapter 3 - update") {
  app_fixture("update", "-u");
}