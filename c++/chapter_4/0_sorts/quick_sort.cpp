#define CATCH_CONFIG_MAIN
#include "../../testlib/catch.hpp"

namespace stiX {
  template<class Iterator>
  void quick_sort(Iterator begin, Iterator end) {
    if (std::distance(begin, end) <= 1)
      return;
    
    throw std::runtime_error("Oops");
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
}