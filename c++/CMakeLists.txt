cmake_minimum_required(VERSION 3.14)
project(sticpp)

set(CMAKE_CXX_STANDARD 17)

add_library(testsupport STATIC
        testlib/testlib.h
        testlib/label_escape.cpp
        testlib/label_escape.h
        testlib/test_filter.cpp
        testlib/test_filter.h)

add_executable(1_copy chapter_1/1_copy/1_copy.cpp chapter_1/1_copy/copy.cpp chapter_1/1_copy/copy.h)
add_executable(2_charcount chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.h chapter_1/2_charcount/2_charcount.cpp)
add_executable(3_linecount chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.h chapter_1/3_linecount/3_linecount.cpp)
add_executable(4_wordcount chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.h chapter_1/4_wordcount/4_wordcount.cpp)
add_executable(5_detab chapter_1/5_detab/5_detab.cpp chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.h lib/tab_stops.h lib/chars.h)
add_executable(5_detab_test chapter_1/5_detab/test.cpp chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.h lib/tab_stops.h lib/chars.h)
target_link_libraries(5_detab_test testsupport)

add_executable(1_entab chapter_2/1_entab/1_entab.cpp chapter_2/1_entab/entab.cpp chapter_2/1_entab/entab.h lib/tab_stops.h lib/chars.h)
add_executable(1_entab_test chapter_2/1_entab/test.cpp chapter_2/1_entab/entab.cpp chapter_2/1_entab/entab.h)
target_link_libraries(1_entab_test testsupport)

add_executable(2_overstrike chapter_2/2_overstrike/2_overstrike.cpp chapter_2/2_overstrike/overstrike.cpp chapter_2/2_overstrike/overstrike.h)
add_executable(2_overstrike_test chapter_2/2_overstrike/test.cpp chapter_2/2_overstrike/overstrike.cpp chapter_2/2_overstrike/overstrike.h)
target_link_libraries(2_overstrike_test testsupport)

add_executable(chapter_1_test
        chapter_1/test.cpp
        chapter_1/1_copy/copy.cpp chapter_1/1_copy/copy.h
        chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.h
        chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.h
        chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.h
        chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.h
        lib/tab_stops.h
        lib/chars.h
    )
target_link_libraries(chapter_1_test testsupport)
