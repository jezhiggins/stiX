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
#include "../../lib/arguments.hpp"

namespace fs = std::filesystem;

int main(int argc, char const* argv[]) {
  auto filenames = stiX::make_arguments(argc, argv);

  if (filenames.size() != 2) {
    std::cerr << "Error: makecopy old new\n";
    return 0;
  }

  auto source = fs::path(filenames[0]);
  auto destination = fs::path(filenames[1]);

  if (fs::equivalent(source, destination)) {
    std::cerr << "Error: source and destination are the same file\n";
    return 0;
  }

  try {
    if (fs::exists(destination) && fs::is_regular_file(destination))
      fs::remove(destination);

    fs::copy_file(source, destination);
  } catch (const fs::filesystem_error& fse) {
    std::cerr << fse.what() << '\n';
  }
}