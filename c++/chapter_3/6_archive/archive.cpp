#include "./archive.hpp"
#include "./create.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void create(std::string const& archive, std::vector<std::string> const& files);
void print_help();

void stiX::archive(std::vector<std::string> const& arguments) {
  if (arguments.size() < 2) {
    print_help();
    return;
  }

  auto const cmd = arguments[0];
  auto const archive = arguments[1];
  auto const files = std::vector<std::string>(arguments.begin() + 2, arguments.end());

  if (cmd == "-c")
    create(archive, files);
  else
    print_help();
} // archive

std::vector<stiX::archive_file> gather_input_files(std::vector<std::string> const& files);
fs::path working_file();

void create(std::string const& archive, std::vector<std::string> const& files) {
  auto input_files = gather_input_files(files);
  auto archive_file = working_file();

  auto archive_out = std::ofstream(archive_file);
  stiX::create_archive(input_files, archive_out);

  fs::rename(archive_file, fs::path(archive));
} // create

void print_help() {
  std::cout << R"c( archive -cmd aname [ file ... ]

 archive manages any number of member files in a single
 file, aname, with sufficient information that members
 may be selectively added, extracted, replaced, or
 deleted from the collection. -cmd is a code that
 determines the operation to be performed.

  -c  create a new archive with named members
  -d  deleted named members from archive
  -p  print named members on standard output
  -t  print table of archive contents
  -u  update named members or add at end
  -x  extract named members from archive
)c";
}

std::vector<stiX::archive_file> gather_input_files(std::vector<std::string> const& files) {
  auto input_files = std::vector<stiX::archive_file>();

  for (auto f : files) {
    auto path = fs::path(f);

    input_files.push_back({ path.string(), fs::file_size(path) });
  }

  return input_files;
} // gather_input_file

fs::path working_file() {
  return fs::temp_directory_path() / "working_archive";
} // working_file
