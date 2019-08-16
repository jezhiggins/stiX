#include <vector>
#include <string>
#include "sample_strings.h"

const std::string empty;
const std::string one_character("1");
const std::string one_word_left_justified("Hello     ");
const std::string one_word_right_justified("     Hello");
const std::string one_word_centred("  hello  ");
const std::string longer("Hello World");
const std::string with_terminal_line_break("The End\n");
const std::string with_leading_line_break("\nThe Beginning");
const std::string line_breaks_before_and_aft("\n  In the middle \n");
const std::string longer_with_multiple_line_breaks("\tHello\n\t\tWorld!\n");

const std::vector<std::string> test_strings = {
        empty,
        one_character,
        one_word_left_justified,
        one_word_right_justified,
        one_word_centred,
        longer,
        with_terminal_line_break,
        with_leading_line_break,
        line_breaks_before_and_aft,
        longer_with_multiple_line_breaks
};

const std::vector<std::string>& sample_strings() {
    return test_strings;
}