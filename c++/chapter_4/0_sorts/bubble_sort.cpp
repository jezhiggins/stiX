#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"
#include <iterator>
#include <list>

namespace stiX {
  template<class Iterator, class Comparator = std::less<>>
  void bubble_sort(Iterator begin, Iterator end, Comparator comparator = Comparator()) {
    if (std::distance(begin, end) <= 0) return;

    for (auto boundary = std::prev(end); boundary != begin; boundary = std::prev(boundary)) {
      for (auto cur = begin; cur != boundary; cur = std::next(cur)) {
        auto next = std::next(cur);
        if (comparator(*next, *cur)) {
          std::iter_swap(next, cur);
        } // if ...
      } // for ...
    } // for ...
  }

  template<class Container, class Comparator = std::less<>>
  void bubble_sort(Container&& sample, Comparator comparator = Comparator()) {
    bubble_sort(std::begin(sample), std::end(sample), comparator);
  }
}

TEST_CASE("Chapter 4 - bubble_sort") {
  SECTION("sorting integers") {
    auto sample = std::vector { 4, 5, 3, 2, 1 };

    stiX::bubble_sort(sample);

    auto expected = std::vector { 1, 2, 3, 4, 5 };
    REQUIRE(sample == expected);
  }

  SECTION("sorting strings") {
    auto sample = std::vector<std::string> { "abc", "aab", "aaa" };

    stiX::bubble_sort(sample);

    auto expected = std::vector<std::string> { "aaa", "aab", "abc" };
    REQUIRE(sample == expected);
  }

  SECTION("sorting list of integers") {
    auto sample = std::list { 4, 5, 3, 2, 1 };

    stiX::bubble_sort(sample.begin(), sample.end());

    auto expected = std::list { 1, 2, 3, 4, 5 };
    REQUIRE(sample == expected);
  }

  SECTION("custom comparator") {
    auto sample = std::vector { 3, 4, 2, 1, 5 };
    auto exemplar = sample;

    stiX::bubble_sort(sample, std::greater<>());
    std::sort(exemplar.begin(), exemplar.end(), std::greater<>());

    REQUIRE(sample == exemplar);
  }

  SECTION("sort an array") {
    int sample[] = { 3, 4, 2, 1, 5 };

    stiX::bubble_sort(sample);

    int expected[] = { 1, 2, 3, 4, 5 };
    for (auto i = 0; i != 5; ++i)
      REQUIRE(sample[i] == expected[i]);
  }

  SECTION("empty container") {
    auto sample = std::vector<int> {};

    stiX::bubble_sort(sample);
  }
}