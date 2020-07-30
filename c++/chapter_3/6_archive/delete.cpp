#include "delete.hpp"
#include <iostream>
#include <algorithm>

#include "../../lib/getline.hpp"
#include "./archive_file.hpp"

bool should_remove(
  std::vector<std::string> const& files_to_remove,
  stiX::archive_file const& name
);

void skip_entry(
  std::istream& archive_in,
  stiX::archive_file const& header
);

void copy_to_out(
  std::istream& archive_in,
  stiX::archive_file const& header,
  std::ostream& archive_out
);

void stiX::delete_from_archive(
  std::istream& archive_in,
  std::vector<std::string> const& files_to_remove,
  std::ostream& archive_out
) {
  archive_in.peek();

  while (archive_in && !archive_in.eof()) {
    auto header_line = getline(archive_in);
    auto header = parse_header(header_line);

    if (should_remove(files_to_remove, header))
      skip_entry(archive_in, header);
    else
      copy_to_out(archive_in, header, archive_out);

    archive_in.peek();
  } // while ...
} // delete_from_archive

bool should_remove(
  std::vector<std::string> const& files_to_remove,
  stiX::archive_file const& header
) {
  return std::find(files_to_remove.begin(), files_to_remove.end(), header.name) != files_to_remove.end();
} // should_remove

void skip_entry(
  std::istream& archive_in,
  stiX::archive_file const& header
) {
  archive_in.seekg(header.filesize, std::ios_base::cur);
} // skip_entry

void copy_to_out(
  std::istream& archive_in,
  stiX::archive_file const& header,
  std::ostream& archive_out
) {
  archive_out << header;

  std::copy_n(
    std::istreambuf_iterator<char>(archive_in),
    header.filesize,
    std::ostreambuf_iterator<char>(archive_out)
  );

  archive_in.get();
} // copy_to_out

