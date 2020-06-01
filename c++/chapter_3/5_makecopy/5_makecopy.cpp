//
// makecopy
//
// makecopy old new
//
// makecopy copes the file old to a new instance of the
// file new, i.e. if new already exists it is truncated and
// rewritten, otherwise it is made to exist. The new file
// is an exact replica of the old.

#include <iostream>
#include <filesystem>
#include <tuple>
#include "../../lib/arguments.hpp"

namespace fs = std::filesystem;

std::tuple<fs::path, fs::path> file_paths(int argc, char const* argv[]);

int main(int argc, char const* argv[]) {
  try {
    auto [source, destination] = file_paths(argc, argv);

    if (fs::exists(destination) && fs::is_regular_file(destination))
      fs::remove(destination);

    fs::copy_file(source, destination);
  } catch (const std::exception& fse) {
    std::cerr << fse.what() << '\n';
  }
}

std::tuple<fs::path, fs::path> file_paths(int argc, char const* argv[]) {
  auto filenames = stiX::make_arguments(argc, argv);

  if (filenames.size() != 2)
    throw std::runtime_error("Error: makecopy old new");

  auto source = fs::path(filenames[0]);
  auto destination = fs::path(filenames[1]);

  if (fs::equivalent(source, destination))
    throw std::runtime_error("Error: source and destination are the same file");

  return std::make_tuple(source, destination);
} // file_paths
