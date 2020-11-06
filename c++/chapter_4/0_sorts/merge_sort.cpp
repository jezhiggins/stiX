#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"
#include <iterator>

namespace stiX {
  template<class Iterator, class Comparator = std::less<>>
  void insertion_sort(Iterator begin, Iterator end, size_t gap, Comparator comparator = Comparator()) {
    auto boundary = std::distance(begin, end) - gap;
    auto cur = begin;
    for (auto i = 0; i < boundary; i += gap) {
      auto ref = cur;
      auto next = std::next(ref, gap);
      while ((next != begin) && comparator(*next, *ref)) {
        auto prev = std::prev(ref, gap);
        std::iter_swap(ref, next);
        next = ref;
        ref = prev;
      }
      cur = std::next(cur, gap);
    }
  } // insertion_sort

  template<class Container, class Comparator = std::less<>>
  void insertion_sort(Container&& sample, size_t gap, Comparator comparator = Comparator()) {
    insertion_sort(std::begin(sample), std::end(sample), gap, comparator);
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
      stiX::insertion_sort(under_test, 1);

      REQUIRE(under_test == expected);
    }
  }

  for (auto sample : samples) {
    auto expected = sample;
    std::sort(std::begin(expected), std::end(expected), std::greater<>());

    DYNAMIC_SECTION("reverse sort " << as_string(sample)) {
      auto under_test = sample;
      stiX::insertion_sort(under_test, 1, std::greater<>());

      REQUIRE(under_test == expected);
    }
  }

  SECTION("sort { 3, 8, 2, 9, 1 } with gap 2") {
    auto sample = std::vector { 3, 9, 2, 8, 1 };
    stiX::insertion_sort(sample, 2);

    REQUIRE(sample == std::vector { 1, 9, 2, 8, 3 });
  }

}