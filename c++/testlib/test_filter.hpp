#ifndef STICPP_TEST_FILTER_HPP
#define STICPP_TEST_FILTER_HPP

void testFilter(
    std::string label,
    std::string input,
    std::string expected,
    std::function<void(std::istream&, std::ostream&)> fn
);

#endif //STICPP_TEST_FILTER_HPP
