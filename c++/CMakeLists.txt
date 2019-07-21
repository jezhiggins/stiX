cmake_minimum_required(VERSION 3.14)
project(sticpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(1_copy chapter_1/1_copy/1_copy.cpp chapter_1/1_copy/copy.cpp chapter_1/1_copy/copy.h)
add_executable(2_charcount chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.h chapter_1/2_charcount/2_charcount.cpp)
add_executable(3_linecount chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.h chapter_1/3_linecount/3_linecount.cpp)
add_executable(4_wordcount chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.h chapter_1/4_wordcount/4_wordcount.cpp)
add_executable(chapter_1_test
        chapter_1/test.cpp
        chapter_1/1_copy/copy.cpp chapter_1/1_copy/copy.h
        chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.h
        chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.h
        chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.h
    )
