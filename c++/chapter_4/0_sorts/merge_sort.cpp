#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"
#include <iterator>

namespace stiX {
  template<class Iterator, class Comparator>
  void back_propagate(Iterator current, Iterator const begin, size_t gap, Comparator comparator) {
    if (current == begin)
      return;

    auto prev = std::prev(current, gap);
    if (comparator(*current, *prev)) {
      std::iter_swap(prev, current);
      back_propagate(prev, begin, gap, comparator);
    }
  }

  template<class Iterator, class Comparator = std::less<>>
  void insertion_sort(Iterator begin, Iterator end, size_t gap, Comparator comparator = Comparator()) {
    auto length = std::distance(begin, end);
    if (length <= gap)
      return;

    auto steps = length - gap;
    auto current = begin;
    for (auto i = 0; i < steps; i += gap) {
      auto next = std::next(current, gap);

      if (comparator(*next, *current)) {
        std::iter_swap(current, next);
        back_propagate(current, begin, gap, comparator);
      }

      current = next;
    }
  } // insertion_sort

  template<class Container, class Comparator = std::less<>>
  void insertion_sort(Container&& sample, size_t gap, Comparator comparator = Comparator()) {
    insertion_sort(std::begin(sample), std::end(sample), gap, comparator);
  }
}

std::string as_string(auto sample) {
  auto out = std::ostringstream();
  out << "{ ";

  auto delim = "";
  for (auto i : sample) {
    out << delim << i;
    delim = ", ";
  }
  out << " }";
  return out.str();
}

TEST_CASE("Chapter 4 - insertion sort") {
  const auto samples = std::vector<std::vector<int>> {
    { },
    { 1 },
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

  SECTION("sort { 3, 8, 2, 9, 1 } with gap 2, offset 1") {
    auto sample = std::vector { 3, 9, 2, 8, 1 };
    stiX::insertion_sort(
        sample.begin() + 1,
        sample.end(),
        2
    );

    REQUIRE(sample == std::vector { 3, 8, 2, 9, 1 });
  }

  auto long_samples = std::vector<std::vector<int>> {
      {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
      {3, 10, 9, 12, 8, 4, 1, 6, 5, 13, 2, 7, 11}
  };

  for (auto sample : long_samples) {
    auto expected = sample;
    std::sort(std::begin(expected), std::end(expected));

    DYNAMIC_SECTION("apply multiple passes, sort " << as_string(sample)) {
      auto under_test = sample;
      for (auto i = 0; i != 7; ++i)
        stiX::insertion_sort(under_test.begin() + i, under_test.end(), 6);
      for (auto i = 0; i != 4; ++i)
        stiX::insertion_sort(under_test.begin() + i, under_test.end(), 3);

      stiX::insertion_sort(under_test.begin(), under_test.end(), 1);

      REQUIRE(under_test == expected);
    }
  }
}