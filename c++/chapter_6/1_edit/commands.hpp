#ifndef STICPP_COMMANDS_HPP
#define STICPP_COMMANDS_HPP

#include "command.hpp"

namespace stiX {
  class commands {
  public:
    commands(command c) :
      commands_() {
      commands_.emplace_back(c);
    }
    commands(std::vector<command>&& cs) :
      commands_(cs) {
    }

    void operator()(
      std::istream& in,
      std::ostream& out,
      edit_buffer& buffer,
      std::string& filename) const;

  private:
    std::vector<command> commands_;
  };
}

#endif //STICPP_COMMANDS_HPP
