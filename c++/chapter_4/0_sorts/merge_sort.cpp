#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"
#include <iterator>

namespace stiX {
  template<class Iterator, class Comparator = std::less<>>
  void insertion_sort(Iterator begin, Iterator end, Comparator comparator = Comparator()) {

    for (auto cur = begin; cur != std::prev(end); cur = std::next(cur)) {
      auto ref = cur;
      auto next = std::next(ref);
      while ((next != begin) && comparator(*next, *ref)) {
        auto prev = std::prev(ref);
        std::iter_swap(ref, next);
        next = ref;
        ref = prev;
      }
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
    { 1, 3, 2 },
    { 3, 2, 1 },
    { 5, 4, 1, 2, 3 }
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

  for (auto sample : samples) {
    auto expected = sample;
    std::sort(std::begin(expected), std::end(expected), std::greater<>());

    DYNAMIC_SECTION("reverse sort " << as_string(sample)) {
      auto under_test = sample;
      stiX::insertion_sort(under_test, std::greater<>());

      REQUIRE(under_test == expected);
    }
  }
}