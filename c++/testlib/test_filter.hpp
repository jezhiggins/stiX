#ifndef STICPP_TEST_FILTER_HPP
#define STICPP_TEST_FILTER_HPP

void testFilter(
    std::string label,
    std::string input,
    std::string expected,
    void (*fn)(std::istream&, std::ostream&)
);

#endif //STICPP_TEST_FILTER_HPP
