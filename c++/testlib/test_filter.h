#ifndef STICPP_TEST_FILTER_H
#define STICPP_TEST_FILTER_H

void testFilter(
    std::string label,
    std::string input,
    std::string expected,
    void (*fn)(std::istream&, std::ostream&)
);

#endif //STICPP_TEST_FILTER_H
