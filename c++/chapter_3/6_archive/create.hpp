#ifndef STICPP_ARCHIVE_CREATE_HPP
#define STICPP_ARCHIVE_CREATE_HPP

#include "../../lib/copy.hpp"
#include <iosfwd>
#include <string>
#include <vector>
#include <iostream>

namespace stiX {
    struct input_file {
        std::string name;
        size_t filesize;
    };

    void write_header(
        stiX::input_file const& input,
        std::ostream& archive_out
    ); // write_header

    template<typename FileReader>
    void write_contents(
        stiX::input_file const& input,
        std::ostream& archive_out,
        FileReader fileReader
    ) {
      auto inputStream = fileReader(input.name);
      copy(inputStream, archive_out);
    } // write_contents

    template<typename FileReader>
    void create_archive(
        std::vector<input_file> const& input,
        std::ostream& archive_out,
        FileReader fileReader
    ) {
      for (auto i : input) {
        write_header(i, archive_out);
        write_contents(i, archive_out, fileReader);
      } // for ...
    } // create_archive
} // namespace stiX
#endif