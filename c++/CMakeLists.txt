cmake_minimum_required(VERSION 3.14)
project(sticpp)

set(CMAKE_CXX_STANDARD 20)

add_library(stix STATIC
        lib/arguments.cpp
        lib/arguments.hpp
        lib/chars.hpp
        lib/copy.cpp
        lib/copy.hpp
        lib/filter.hpp
        lib/tab_stops.hpp
        lib/getline.cpp
        lib/getline.hpp
        lib/file_open.hpp
        lib/file_open.cpp)

add_library(testsupport STATIC
        testlib/testlib.hpp
        testlib/label_escape.cpp
        testlib/label_escape.hpp
        testlib/test_filter.cpp
        testlib/test_filter.hpp
        testlib/sample_strings.cpp
        testlib/sample_strings.hpp
        testlib/verify_with_samples.cpp
        testlib/verify_with_samples.hpp)

add_executable(1_copy chapter_1/1_copy/1_copy.cpp)
target_link_libraries(1_copy stix)
add_executable(1_copy_test chapter_1/1_copy/test.cpp)
target_link_libraries(1_copy_test testsupport stix)

add_executable(2_charcount chapter_1/2_charcount/2_charcount.cpp chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.hpp)
add_executable(2_charcount_test chapter_1/2_charcount/test.cpp chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.hpp)
target_link_libraries(2_charcount_test testsupport)

add_executable(3_linecount chapter_1/3_linecount/3_linecount.cpp chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.hpp)
add_executable(3_linecount_test chapter_1/3_linecount/test.cpp chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.hpp)
target_link_libraries(3_linecount_test testsupport)

add_executable(4_wordcount chapter_1/4_wordcount/4_wordcount.cpp chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.hpp)
add_executable(4_wordcount_test chapter_1/4_wordcount/test.cpp chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.hpp)
target_link_libraries(4_wordcount_test testsupport)

add_executable(5_detab chapter_1/5_detab/5_detab.cpp chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.hpp lib/tab_stops.hpp lib/chars.hpp)
add_executable(5_detab_test chapter_1/5_detab/test.cpp chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.hpp lib/tab_stops.hpp lib/chars.hpp)
target_link_libraries(5_detab_test testsupport)

add_executable(1_entab chapter_2/1_entab/1_entab.cpp chapter_2/1_entab/entab.cpp chapter_2/1_entab/entab.hpp lib/tab_stops.hpp lib/chars.hpp lib/filter.hpp)
add_executable(1_entab_test chapter_2/1_entab/test.cpp chapter_2/1_entab/entab.cpp chapter_2/1_entab/entab.hpp lib/filter.hpp)
target_link_libraries(1_entab_test testsupport)

add_executable(2_overstrike chapter_2/2_overstrike/2_overstrike.cpp chapter_2/2_overstrike/overstrike.cpp chapter_2/2_overstrike/overstrike.hpp lib/filter.hpp)
add_executable(2_overstrike_test chapter_2/2_overstrike/test.cpp chapter_2/2_overstrike/overstrike.cpp chapter_2/2_overstrike/overstrike.hpp lib/filter.hpp)
target_link_libraries(2_overstrike_test testsupport)

add_executable(3_compress chapter_2/3_compress/3_compress.cpp chapter_2/3_compress/compress.cpp chapter_2/3_compress/compress.hpp lib/filter.hpp)
add_executable(3_compress_test chapter_2/3_compress/test.cpp chapter_2/3_compress/compress.cpp chapter_2/3_compress/compress.hpp lib/filter.hpp)
target_link_libraries(3_compress_test testsupport)

add_executable(4_expand chapter_2/4_expand/4_expand.cpp chapter_2/4_expand/expand.cpp chapter_2/4_expand/expand.hpp lib/filter.hpp)
add_executable(4_expand_test chapter_2/4_expand/test.cpp chapter_2/4_expand/expand.cpp chapter_2/4_expand/expand.hpp lib/filter.hpp)
target_link_libraries(4_expand_test testsupport)

add_executable(5_echo chapter_2/5_echo/5_echo.cpp chapter_2/5_echo/echo.cpp chapter_2/5_echo/echo.hpp)
target_link_libraries(5_echo stix)
add_executable(5_echo_test chapter_2/5_echo/test.cpp chapter_2/5_echo/echo.cpp chapter_2/5_echo/echo.hpp)
target_link_libraries(5_echo_test stix testsupport)

add_executable(6_translit chapter_2/6_translit/6_translit.cpp chapter_2/6_translit/translit.cpp chapter_2/6_translit/translit.hpp chapter_2/6_translit/translit-args.cpp chapter_2/6_translit/translit-args.hpp)
target_link_libraries(6_translit stix)
add_executable(6_translit_test chapter_2/6_translit/test.cpp chapter_2/6_translit/translit.cpp chapter_2/6_translit/translit.hpp chapter_2/6_translit/translit-args.cpp chapter_2/6_translit/translit-args.hpp)
target_link_libraries(6_translit_test stix testsupport)

add_executable(1_compare chapter_3/1_compare/1_compare.cpp chapter_3/1_compare/compare.cpp chapter_3/1_compare/compare.hpp chapter_3/6_archive/test/test_print.inc)
target_link_libraries(1_compare stix)
add_executable(1_compare_test chapter_3/1_compare/test.cpp chapter_3/1_compare/compare.cpp chapter_3/1_compare/compare.hpp)
target_link_libraries(1_compare_test stix testsupport)

add_executable(2_include chapter_3/2_include/2_include.cpp chapter_3/2_include/include.cpp chapter_3/2_include/include.hpp)
target_link_libraries(2_include stix)
add_executable(2_include_test chapter_3/2_include/test.cpp chapter_3/2_include/include.cpp chapter_3/2_include/include.hpp)
target_link_libraries(2_include_test stix testsupport)

add_executable(3_concat chapter_3/3_concat/3_concat.cpp)
target_link_libraries(3_concat stix)

add_executable(4_print chapter_3/4_print/4_print.cpp chapter_3/4_print/print.cpp chapter_3/4_print/print.hpp)
target_link_libraries(4_print stix)
add_executable(4_print_test chapter_3/4_print/test.cpp chapter_3/4_print/print.cpp chapter_3/4_print/print.hpp)
target_link_libraries(4_print_test stix testsupport)

add_executable(5_makecopy chapter_3/5_makecopy/5_makecopy.cpp)
target_link_libraries(5_makecopy stix)

add_executable(6_archive chapter_3/6_archive/6_archive.cpp chapter_3/6_archive/append.hpp chapter_3/6_archive/table.hpp chapter_3/6_archive/table.cpp chapter_3/6_archive/archive_file.hpp chapter_3/6_archive/archive.hpp chapter_3/6_archive/archive.cpp chapter_3/6_archive/delete.hpp chapter_3/6_archive/delete.cpp chapter_3/6_archive/archive_file.cpp chapter_3/6_archive/extract.hpp chapter_3/6_archive/extract.cpp)
target_link_libraries(6_archive stix)
add_executable(6_archive_test chapter_3/6_archive/test.cpp chapter_3/6_archive/test/test_create.inc chapter_3/6_archive/append.hpp chapter_3/1_compare/compare.cpp chapter_3/1_compare/compare.hpp chapter_3/6_archive/test/test_table.inc chapter_3/6_archive/table.hpp chapter_3/6_archive/table.cpp chapter_3/6_archive/archive_file.hpp chapter_3/6_archive/archive.hpp chapter_3/6_archive/archive.cpp chapter_3/6_archive/test/test_delete.inc chapter_3/6_archive/delete.hpp chapter_3/6_archive/delete.cpp chapter_3/6_archive/archive_file.cpp chapter_3/6_archive/test/test_print.inc chapter_3/6_archive/test/test_update.inc chapter_3/6_archive/test/test_extract.inc chapter_3/6_archive/extract.hpp chapter_3/6_archive/extract.cpp)
target_link_libraries(6_archive_test stix testsupport)

add_executable(0_bubble_sort chapter_4/0_sorts/bubble_sort.cpp)
target_link_libraries(0_bubble_sort)

add_executable(0_shell_sort chapter_4/0_sorts/shell_sort.cpp)
target_link_libraries(0_shell_sort)

add_executable(1_sort chapter_4/1_sort/1_sort.cpp)
target_link_libraries(1_sort stix)

add_executable(0_quick_sort chapter_4/0_sorts/quick_sort.cpp)
target_link_libraries(0_quick_sort)

add_executable(2_external_sort
        chapter_4/2_external_sort/2_external_sort.cpp
        chapter_4/2_external_sort/working_filepaths.hpp
        chapter_4/2_external_sort/working_filepaths.cpp
        chapter_4/2_external_sort/read_to_files.hpp
        chapter_4/2_external_sort/read_to_files.cpp chapter_4/2_external_sort/merge_files.cpp chapter_4/2_external_sort/merge_files.hpp)
target_link_libraries(2_external_sort stix)
add_executable(2_external_sort_test chapter_4/2_external_sort/test.cpp chapter_4/2_external_sort/working_filepaths.hpp chapter_4/2_external_sort/working_filepaths.cpp)
target_link_libraries(2_external_sort_test stix testsupport)

add_executable(3_unique
        chapter_4/3_unique/3_unique.cpp
        chapter_4/3_unique/unique.cpp
        chapter_4/3_unique/unique.hpp)
target_link_libraries(3_unique stix)
add_executable(3_unique_test
        chapter_4/3_unique/test.cpp
        chapter_4/3_unique/unique.cpp
        chapter_4/3_unique/unique.hpp)
target_link_libraries(3_unique_test stix testsupport)

add_executable(4_kwic
        chapter_4/4_kwic/4_kwic.cpp
        chapter_4/4_kwic/kwic.cpp
        chapter_4/4_kwic/kwic.hpp)
target_link_libraries(4_kwic stix)
add_executable(4_kwic_test
        chapter_4/4_kwic/test.cpp
        chapter_4/4_kwic/kwic.cpp
        chapter_4/4_kwic/kwic.hpp)
target_link_libraries(4_kwic_test stix testsupport)

