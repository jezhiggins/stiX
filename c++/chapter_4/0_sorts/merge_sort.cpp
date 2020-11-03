#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"
#include <iterator>

namespace stiX {
  template<class Iterator, class Comparator = std::less<>>
  void insertion_sort(Iterator begin, Iterator end, Comparator comparator = Comparator()) {

    for (auto cur = begin; cur != std::prev(end); cur = std::next(cur)) {
      auto next = std::next(cur);
      if (*next < *cur)
        std::iter_swap(cur, next);
    }
  } // insertion_sort

  template<class Container, class Comparator = std::less<>>
  void insertion_sort(Container&& sample, Comparator comparator = Comparator()) {
    insertion_sort(std::begin(sample), std::end(sample), comparator);
  }
}

std::string as_string(auto sample) {
  auto out = std::ostringstream();
  auto delim = "{ ";
  for (auto i : sample) {
    out << delim << i;
    delim = ", ";
  }
  out << " }";
  return out.str();
}

TEST_CASE("Chapter 4 - insertion sort") {
  const auto samples = std::vector<std::vector<int>> {
    { 1, 2 },
    { 2, 1 },
    { 1, 3, 2 }
  };

  for (auto sample : samples) {
    auto expected = sample;
    std::sort(std::begin(expected), std::end(expected));

    DYNAMIC_SECTION("sort " << as_string(sample)) {
      auto under_test = sample;
      stiX::insertion_sort(under_test);

      REQUIRE(under_test == expected);
    }
  }
}