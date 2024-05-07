#ifndef STICPP_COMMANDS_HPP
#define STICPP_COMMANDS_HPP

#include "command.hpp"
#include <initializer_list>

namespace stiX {
  class commands {
  public:
    commands(std::initializer_list<command> cs) :
      commands_(cs) {
    }

    void operator()(
      std::istream& in,
      std::ostream& out,
      lines_modifier& buffer,
      std::string& filename) const;

  private:
    std::vector<command> commands_;

    friend std::vector<command> const& command_list(commands const&);
  };
}

#endif //STICPP_COMMANDS_HPP
