#include "./print.hpp"

#include "./archive_file.hpp"

namespace stiX {
  void print_files(
    std::istream& archive_in,
    std::vector<std::string> const& files,
    std::ostream& out
  ) {
    read_archive(
        archive_in,
        [&files, &out](
          std::istream& archive_in,
          stiX::archive_file const& header
        ) {
           if (of_interest(files, header))
             copy_contents(archive_in, header, out);
           else
             skip_entry(archive_in, header);
       }
    );
  } // print_files
} // namespace stiX