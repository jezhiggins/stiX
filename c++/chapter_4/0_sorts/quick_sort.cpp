#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"
#include <iterator>

namespace stiX {
  template<class Iterator>
  void quick_sort(Iterator begin, Iterator end) {
    if(std::distance(begin, end) <= 1)
      return;

    auto pivot = *begin;
    auto left = std::next(begin);
    auto right = std::prev(end);

    while (left != right) {
      while ((left != right) && (*left < pivot))
        left = std::next(left);

      while ((left != right) && (*right >= pivot))
        right = std::prev(right);

      if (left != right)
        std::iter_swap(left, right);
    }

    left = std::prev(left);
    std::iter_swap(left, begin);

    quick_sort(begin, left);
    quick_sort(right, end);
  } // quick_sort

  template<class Container>
  void quick_sort(Container&& sample) {
    quick_sort(std::begin(sample), std::end(sample));
  }
} // namespace stiX

TEST_CASE("Chapter 4 - quick sort") {
  SECTION("empty list") {
    auto sample = std::vector<int> { };
    stiX::quick_sort(sample);
  }

  SECTION("one item") {
    auto sample = std::vector { 1 };
    stiX::quick_sort(sample);

    REQUIRE(sample == std::vector { 1 });
  }

  SECTION("first partition") {
    auto sample = std::vector { 4, 9, 1, 8, 2, 7 };
    stiX::quick_sort(sample);

    REQUIRE(sample == std::vector { 1, 2, 4, 7, 8, 9 });
  }
}