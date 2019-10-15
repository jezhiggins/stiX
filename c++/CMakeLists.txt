cmake_minimum_required(VERSION 3.14)
project(sticpp)

set(CMAKE_CXX_STANDARD 17)

add_library(testsupport STATIC
        testlib/testlib.h
        testlib/label_escape.cpp
        testlib/label_escape.h
        testlib/test_filter.cpp
        testlib/test_filter.h
        testlib/sample_strings.cpp
        testlib/sample_strings.h
        testlib/verify_with_samples.cpp
        testlib/verify_with_samples.h)

add_executable(1_copy chapter_1/1_copy/1_copy.cpp chapter_1/1_copy/copy.cpp chapter_1/1_copy/copy.h)
add_executable(1_copy_test chapter_1/1_copy/test.cpp chapter_1/1_copy/copy.cpp chapter_1/1_copy/copy.h)
target_link_libraries(1_copy_test testsupport)

add_executable(2_charcount chapter_1/2_charcount/2_charcount.cpp chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.h)
add_executable(2_charcount_test chapter_1/2_charcount/test.cpp chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.h)
target_link_libraries(2_charcount_test testsupport)

add_executable(3_linecount chapter_1/3_linecount/3_linecount.cpp chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.h)
add_executable(3_linecount_test chapter_1/3_linecount/test.cpp chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.h)
target_link_libraries(3_linecount_test testsupport)

add_executable(4_wordcount chapter_1/4_wordcount/4_wordcount.cpp chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.h)
add_executable(4_wordcount_test chapter_1/4_wordcount/test.cpp chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.h)
target_link_libraries(4_wordcount_test testsupport)

add_executable(5_detab chapter_1/5_detab/5_detab.cpp chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.h lib/tab_stops.h lib/chars.h)
add_executable(5_detab_test chapter_1/5_detab/test.cpp chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.h lib/tab_stops.h lib/chars.h)
target_link_libraries(5_detab_test testsupport)

add_executable(1_entab chapter_2/1_entab/1_entab.cpp chapter_2/1_entab/entab.cpp chapter_2/1_entab/entab.h lib/tab_stops.h lib/chars.h lib/filter.h)
add_executable(1_entab_test chapter_2/1_entab/test.cpp chapter_2/1_entab/entab.cpp chapter_2/1_entab/entab.h lib/filter.h)
target_link_libraries(1_entab_test testsupport)

add_executable(2_overstrike chapter_2/2_overstrike/2_overstrike.cpp chapter_2/2_overstrike/overstrike.cpp chapter_2/2_overstrike/overstrike.h lib/filter.h)
add_executable(2_overstrike_test chapter_2/2_overstrike/test.cpp chapter_2/2_overstrike/overstrike.cpp chapter_2/2_overstrike/overstrike.h lib/filter.h)
target_link_libraries(2_overstrike_test testsupport)

add_executable(3_compress chapter_2/3_compress/3_compress.cpp chapter_2/3_compress/compress.cpp chapter_2/3_compress/compress.h lib/filter.h)
add_executable(3_compress_test chapter_2/3_compress/test.cpp chapter_2/3_compress/compress.cpp chapter_2/3_compress/compress.h lib/filter.h)
target_link_libraries(3_compress_test testsupport)

add_executable(4_expand_test chapter_2/4_expand/test.cpp chapter_2/4_expand/expand.cpp chapter_2/4_expand/expand.h lib/filter.h)
target_link_libraries(4_expand_test testsupport)

add_executable(5_echo chapter_2/5_echo/5_echo.cpp chapter_2/5_echo/echo.cpp chapter_2/5_echo/echo.h)
add_executable(5_echo_test chapter_2/5_echo/test.cpp chapter_2/5_echo/echo.cpp chapter_2/5_echo/echo.h)
target_link_libraries(5_echo_test testsupport)

add_executable(6_translit chapter_2/6_translit/6_translit.cpp chapter_2/6_translit/translit.cpp chapter_2/6_translit/translit.hpp chapter_2/6_translit/translit-args.cpp chapter_2/6_translit/translit-args.hpp)
add_executable(6_translit_test chapter_2/6_translit/test.cpp chapter_2/6_translit/translit.cpp chapter_2/6_translit/translit.hpp chapter_2/6_translit/translit-args.cpp chapter_2/6_translit/translit-args.hpp)
target_link_libraries(5_echo_test testsupport)