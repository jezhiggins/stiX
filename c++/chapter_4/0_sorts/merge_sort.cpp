#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"

namespace stiX {
  template<class Iterator, class Comparator = std::less<>>
  void insertion_sort(Iterator begin, Iterator end, Comparator comparator = Comparator()) {

  } // insertion_sort

  template<class Container, class Comparator = std::less<>>
  void insertion_sort(Container&& sample, Comparator comparator = Comparator()) {
    insertion_sort(std::begin(sample), std::end(sample), comparator);
  }
}

TEST_CASE("Chapter 4 - insertion sort") {
  SECTION("sort { 1, 2 }") {
    const auto expected = std::vector { 1, 2 };

    auto sample = std::vector { 1, 2 };
    stiX::insertion_sort(sample);
    REQUIRE(sample == expected);
  }
}