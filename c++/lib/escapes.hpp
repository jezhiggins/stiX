#ifndef STICPP_ESCAPES_H
#define STICPP_ESCAPES_H

#include <string>

namespace stiX {
  constexpr char Escape = '@';

  char expand_escape(char candidate);

  char is_valid_escape_sequence(
    std::string::const_iterator const& c,
    std::string::const_iterator const& end
  );
}

#endif //STICPP_ESCAPES_H
