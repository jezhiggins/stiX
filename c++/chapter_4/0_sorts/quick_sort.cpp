#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"
#include <iterator>
#include <list>

using namespace std::literals::string_literals;

namespace stiX {
  template<class Iterator, class Comparator = std::less<>>
  void quick_sort(Iterator begin, Iterator end, Comparator comparator = Comparator()) {
    if(std::distance(begin, end) <= 1)
      return;

    auto pivot = *begin;
    auto left = std::next(begin);
    auto right = std::prev(end);

    while (left != right) {
      while ((left != right) && comparator(*left, pivot))
        left = std::next(left);

      while ((left != right) && !comparator(*right, pivot))
        right = std::prev(right);

      if (left != right)
        std::iter_swap(left, right);
    }

    if (!comparator(*left, pivot))
      left = std::prev(left);
    std::iter_swap(left, begin);

    quick_sort(begin, left, comparator);
    quick_sort(right, end, comparator);
  } // quick_sort

  template<class Container, class Comparator = std::less<>>
  void quick_sort(Container&& sample, Comparator comparator = Comparator()) {
    quick_sort(std::begin(sample), std::end(sample), comparator);
  }
} // namespace stiX

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

auto const samples = std::vector<std::vector<int>> {
  { },
  { 1 },
  { 1, 2 },
  { 2, 1 },
  { 1, 3, 2 },
  { 3, 2, 1 },
  { 5, 4, 1, 2, 3 },
  { 4, 9, 1, 8, 2, 7 }
};

auto const long_samples = std::vector<std::vector<int>> {
  {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1},
  {3, 10, 9, 12, 8, 4, 1, 6, 5, 13, 2, 7, 11}
};

void test_quick_sort(auto sample) {
  DYNAMIC_SECTION("sort " << as_string(sample)) {
    auto expected = sample;
    std::sort(std::begin(expected), std::end(expected));

    auto under_test = sample;
    stiX::quick_sort(under_test);

    REQUIRE(under_test == expected);
  }

  DYNAMIC_SECTION("reverse sort " << as_string(sample)) {
    auto expected = sample;
    std::sort(std::begin(expected), std::end(expected), std::greater<>());

    auto under_test = sample;
    stiX::quick_sort(under_test, std::greater<>());

    REQUIRE(under_test == expected);
  }
}

TEST_CASE("Chapter 4 - quick sort") {
  for (auto sample : samples)
    test_quick_sort(sample);
  for (auto sample : long_samples)
    test_quick_sort(sample);

  SECTION("sort a list") {
    auto sample = std::list{5, 3, 1, 2, 3};
    auto expected = sample;
    expected.sort();

    auto under_test = sample;
    stiX::quick_sort(under_test);

    REQUIRE(under_test == expected);
  }

  SECTION("sort a std::array") {
    auto sample = std::array {5, 3, 1, 2, 3};
    auto expected = sample;
    std::sort(std::begin(expected), std::end(expected));

    auto under_test = sample;
    stiX::quick_sort(under_test);

    REQUIRE(under_test == expected);
  }

  SECTION("sort an array") {
    int under_test[6] = { 7, 3, 0, 8, 1, 2 };

    stiX::quick_sort(under_test);

    int expected[6] = { 0, 1, 2, 3, 7, 8 };
    for (auto i = 0; i != 6; ++i)
      REQUIRE(under_test[i] == expected[i]);
  }

  SECTION("sort some strings") {
    auto sample = std::vector { "dog"s, "aardvark"s, "panda"s, "marmoset"s };
    auto expected = sample;
    std::sort(std::begin(expected), std::end(expected));

    auto under_test = sample;
    stiX::quick_sort(under_test);

    REQUIRE(under_test == expected);
  }
}