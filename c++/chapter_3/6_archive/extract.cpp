#include "./extract.hpp"

namespace stiX {
  template<typename FileWriteOpener>
  void extract_files_to_sink(
    std::istream& archive_in,
    std::vector<std::string> const& files,
    FileWriteOpener file_writer
  ) {
    read_archive(
      archive_in,
      [&files, &file_writer](
        std::istream& archive_in,
        stiX::archive_file const& header
      ) {
        if (of_interest(files, header)) {
          decltype(file_writer(header.name)) out = file_writer(header.name);
          copy_contents(archive_in, header, out);
        }
        else
          skip_entry(archive_in, header);
      }
    );
  } // extract_files_to_sink

  std::ostream& send_to_stdout(std::string const&) {
    return std::cout;
  } // send_to_stdout

  void print_files(
    std::istream& archive_in,
    std::vector<std::string> const& files
  ) {
    extract_files_to_sink(
      archive_in,
      files,
      send_to_stdout
    );
  } // print_files

  void extract_files(
    std::istream& archive_in,
    std::vector<std::string> const& files
  ) {
    extract_files_to_sink(
      archive_in,
      files,
      file_write_opener
    );
  } // extract_files
} // namespace stiX