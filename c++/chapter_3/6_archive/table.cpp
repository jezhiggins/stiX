#include "table.hpp"

#include "./archive_file.hpp"

namespace stiX {
  void table_archive(
    std::istream& archive_in,
    std::ostream& out
  ) {
    read_archive(
    archive_in,
       [&out](
         std::istream& archive_in,
         stiX::archive_file const& header
       ) {
         out << header.name << '\t' << header.filesize << '\n';

         skip_entry(archive_in, header);
       }
    );
  } // table_archive
}
