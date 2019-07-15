cmake_minimum_required(VERSION 3.14)
project(c__)

set(CMAKE_CXX_STANDARD 17)

add_executable(1_copy chapter_1/1_copy/1_copy.cpp chapter_1/1_copy/copy.cpp chapter_1/1_copy/copy.h)
add_executable(1_copy_test chapter_1/1_copy/1_copy_test.cpp chapter_1/1_copy/copy.cpp chapter_1/1_copy/copy.h)