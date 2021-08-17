#ifndef STICPP_DASH_RANGE_HPP
#define STICPP_DASH_RANGE_HPP

namespace stiX {
  const char Dash = '-';
  bool is_dash_range(char from, char to);

  template<class InserterT>
  void expand_dash_range(char from, char to, InserterT dest) {
    while (from <= to)
      dest = from++;
  }
}

#endif //STICPP_DASH_RANGE_HPP
