
#include <string>
#include "label_escape.hpp"
#include <vector>
#include <tuple>

///////////////////
std::vector<std::tuple<std::string, std::string>> sequences = {
    { "\n", "\\n" },
    { "\t", "\\t" },
    { "\b", "\\b" }
};

std::string escape(std::string s) {
  for (auto esc : sequences ) {
    auto f = std::get<0>(esc);
    auto r = std::get<1>(esc);

    for(size_t pos = s.find(f); pos != std::string::npos; pos = s.find(f))
      s.replace(pos, f.size(), r);

  }
  return s;
}


