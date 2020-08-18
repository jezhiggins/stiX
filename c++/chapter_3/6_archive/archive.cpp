#include "./archive.hpp"
#include "./create.hpp"
#include "./table.hpp"
#include "./delete.hpp"
#include "./print.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

void create(std::string const& archive, std::vector<std::string> const& files);
void table(std::string const& archive);
void remove(std::string const& archive, std::vector<std::string> const& files);
void print(std::string const& archive, std::vector<std::string> const& files);
void update(std::string const& archive, std::vector<std::string> const& files);
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
  else if (cmd == "-t")
    table(archive);
  else if (cmd == "-d")
    remove(archive, files);
  else if (cmd == "-p")
    print(archive, files);
  else if (cmd == "-u")
    update(archive, files);
  else
    print_help();
} // archive

std::vector<stiX::archive_file> gather_input_files(std::vector<std::string> const& files);
fs::path working_file();

void create(std::string const& archive, std::vector<std::string> const& files) {
  auto input_files = gather_input_files(files);
  auto working = working_file();

  {
    auto archive_out = std::ofstream(working);
    stiX::create_archive(input_files, archive_out);
  }

  fs::rename(working, archive);
} // create

void table(std::string const& archive) {
  auto archive_in = std::ifstream(archive);
  stiX::table_archive(archive_in, std::cout);
} // table

void remove(std::string const& archive, std::vector<std::string> const& files) {
  auto working = working_file();

  {
    auto archive_in = std::ifstream(archive);
    auto archive_out = std::ofstream(working);
    stiX::delete_from_archive(archive_in, files, archive_out);
  }

  fs::rename(working, archive);
} // delete

void print(std::string const& archive, std::vector<std::string> const& files) {
  auto archive_in = std::ifstream(archive);
  stiX::print_files(archive_in, files, std::cout);
} // print

void update(std::string const& archive, std::vector<std::string> const& files) {
  auto working = working_file();

  {
    auto archive_in = std::ifstream(archive);
    auto archive_out = std::ofstream(working);
    stiX::delete_from_archive(archive_in, files, archive_out);

    auto input_files = gather_input_files(files);
    stiX::create_archive(input_files, archive_out);
  }

  fs::rename(working, archive);
} // update

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
  auto name = std::string { "working_archive_" } + std::to_string(std::rand());
  return fs::temp_directory_path() / name;
} // working_file
