#include "./print.hpp"

#include "./extract.hpp"

namespace stiX {
  std::ostream& send_to_stdout(std::string const&) {
    return std::cout;
  } // send_to_stdout

  void print_files(
    std::istream& archive_in,
    std::vector<std::string> const& files
  ) {
    extract_files(
      archive_in,
      files,
      send_to_stdout
    );
  } // print_files
} // namespace stiX