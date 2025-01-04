cmake_minimum_required(VERSION 3.14)
project(sticpp)

set(CMAKE_CXX_STANDARD 23)

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
        lib/file_open.cpp
        lib/dash_range.cpp
        lib/dash_range.hpp
        lib/escapes.cpp
        lib/escapes.hpp
        lib/regex/matcher.cpp
        lib/regex/matcher.hpp
        lib/regex/pattern_matcher.hpp
        lib/regex/pattern_matcher.cpp
        lib/regex/char_seq.cpp
        lib/regex/char_seq.hpp
        lib/regex/replacement.cpp
	lib/regex/replacement.hpp
	lib/regex/change.cpp
	lib/regex/change.hpp
		lib/chars.cpp)

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

add_executable(copy chapter_1/1_copy/1_copy.cpp)
target_link_libraries(copy stix)
add_executable(copy_test chapter_1/1_copy/test.cpp)
target_link_libraries(copy_test testsupport stix)

add_executable(charcount chapter_1/2_charcount/2_charcount.cpp chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.hpp)
add_executable(charcount_test chapter_1/2_charcount/test.cpp chapter_1/2_charcount/charcount.cpp chapter_1/2_charcount/charcount.hpp)
target_link_libraries(charcount_test testsupport)

add_executable(linecount chapter_1/3_linecount/3_linecount.cpp chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.hpp)
add_executable(linecount_test chapter_1/3_linecount/test.cpp chapter_1/3_linecount/linecount.cpp chapter_1/3_linecount/linecount.hpp)
target_link_libraries(linecount_test testsupport)

add_executable(wordcount chapter_1/4_wordcount/4_wordcount.cpp chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.hpp)
add_executable(wordcount_test chapter_1/4_wordcount/test.cpp chapter_1/4_wordcount/wordcount.cpp chapter_1/4_wordcount/wordcount.hpp)
target_link_libraries(wordcount_test testsupport)

add_executable(detab chapter_1/5_detab/5_detab.cpp chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.hpp lib/tab_stops.hpp lib/chars.hpp)
add_executable(detab_test chapter_1/5_detab/test.cpp chapter_1/5_detab/detab.cpp chapter_1/5_detab/detab.hpp lib/tab_stops.hpp lib/chars.hpp)
target_link_libraries(detab_test testsupport)

add_executable(entab chapter_2/1_entab/1_entab.cpp chapter_2/1_entab/entab.cpp chapter_2/1_entab/entab.hpp lib/tab_stops.hpp lib/chars.hpp lib/filter.hpp)
add_executable(entab_test chapter_2/1_entab/test.cpp chapter_2/1_entab/entab.cpp chapter_2/1_entab/entab.hpp lib/filter.hpp)
target_link_libraries(entab_test testsupport)

add_executable(overstrike chapter_2/2_overstrike/2_overstrike.cpp chapter_2/2_overstrike/overstrike.cpp chapter_2/2_overstrike/overstrike.hpp lib/filter.hpp)
add_executable(overstrike_test chapter_2/2_overstrike/test.cpp chapter_2/2_overstrike/overstrike.cpp chapter_2/2_overstrike/overstrike.hpp lib/filter.hpp)
target_link_libraries(overstrike_test testsupport)

add_executable(compress chapter_2/3_compress/3_compress.cpp chapter_2/3_compress/compress.cpp chapter_2/3_compress/compress.hpp lib/filter.hpp)
add_executable(compress_test chapter_2/3_compress/test.cpp chapter_2/3_compress/compress.cpp chapter_2/3_compress/compress.hpp lib/filter.hpp)
target_link_libraries(compress_test testsupport)

add_executable(expand chapter_2/4_expand/4_expand.cpp chapter_2/4_expand/expand.cpp chapter_2/4_expand/expand.hpp lib/filter.hpp)
add_executable(expand_test chapter_2/4_expand/test.cpp chapter_2/4_expand/expand.cpp chapter_2/4_expand/expand.hpp lib/filter.hpp)
target_link_libraries(expand_test testsupport)

add_executable(echo chapter_2/5_echo/5_echo.cpp chapter_2/5_echo/echo.cpp chapter_2/5_echo/echo.hpp)
target_link_libraries(echo stix)
add_executable(echo_test chapter_2/5_echo/test.cpp chapter_2/5_echo/echo.cpp chapter_2/5_echo/echo.hpp)
target_link_libraries(echo_test stix testsupport)

add_executable(translit
        chapter_2/6_translit/6_translit.cpp
        chapter_2/6_translit/translit.cpp
        chapter_2/6_translit/translit.hpp
        chapter_2/6_translit/translit-args.cpp
        chapter_2/6_translit/translit-args.hpp
        lib/escapes.hpp)
target_link_libraries(translit stix)
add_executable(translit_test
        chapter_2/6_translit/test.cpp
        chapter_2/6_translit/translit.cpp
        chapter_2/6_translit/translit.hpp
        chapter_2/6_translit/translit-args.cpp
        chapter_2/6_translit/translit-args.hpp
        lib/escapes.hpp)
target_link_libraries(translit_test stix testsupport)

add_executable(compare chapter_3/1_compare/1_compare.cpp chapter_3/1_compare/compare.cpp chapter_3/1_compare/compare.hpp chapter_3/6_archive/test/test_print.inc)
target_link_libraries(compare stix)
add_executable(compare_test chapter_3/1_compare/test.cpp chapter_3/1_compare/compare.cpp chapter_3/1_compare/compare.hpp)
target_link_libraries(compare_test stix testsupport)

add_executable(include chapter_3/2_include/2_include.cpp chapter_3/2_include/include.cpp chapter_3/2_include/include.hpp)
target_link_libraries(include stix)
add_executable(include_test chapter_3/2_include/test.cpp chapter_3/2_include/include.cpp chapter_3/2_include/include.hpp)
target_link_libraries(include_test stix testsupport)

add_executable(concat chapter_3/3_concat/3_concat.cpp)
target_link_libraries(concat stix)

add_executable(print chapter_3/4_print/4_print.cpp chapter_3/4_print/print.cpp chapter_3/4_print/print.hpp)
target_link_libraries(print stix)
add_executable(print_test chapter_3/4_print/test.cpp chapter_3/4_print/print.cpp chapter_3/4_print/print.hpp)
target_link_libraries(print_test stix testsupport)

add_executable(makecopy chapter_3/5_makecopy/5_makecopy.cpp)
target_link_libraries(makecopy stix)

add_executable(archive chapter_3/6_archive/6_archive.cpp chapter_3/6_archive/append.hpp chapter_3/6_archive/table.hpp chapter_3/6_archive/table.cpp chapter_3/6_archive/archive_file.hpp chapter_3/6_archive/archive.hpp chapter_3/6_archive/archive.cpp chapter_3/6_archive/delete.hpp chapter_3/6_archive/delete.cpp chapter_3/6_archive/archive_file.cpp chapter_3/6_archive/extract.hpp chapter_3/6_archive/extract.cpp)
target_link_libraries(archive stix)
add_executable(archive_test chapter_3/6_archive/test.cpp chapter_3/6_archive/test/test_create.inc chapter_3/6_archive/append.hpp chapter_3/1_compare/compare.cpp chapter_3/1_compare/compare.hpp chapter_3/6_archive/test/test_table.inc chapter_3/6_archive/table.hpp chapter_3/6_archive/table.cpp chapter_3/6_archive/archive_file.hpp chapter_3/6_archive/archive.hpp chapter_3/6_archive/archive.cpp chapter_3/6_archive/test/test_delete.inc chapter_3/6_archive/delete.hpp chapter_3/6_archive/delete.cpp chapter_3/6_archive/archive_file.cpp chapter_3/6_archive/test/test_print.inc chapter_3/6_archive/test/test_update.inc chapter_3/6_archive/test/test_extract.inc chapter_3/6_archive/extract.hpp chapter_3/6_archive/extract.cpp)
target_link_libraries(archive_test stix testsupport)

add_executable(bubble_sort chapter_4/0_sorts/bubble_sort.cpp)
target_link_libraries(bubble_sort)

add_executable(shell_sort chapter_4/0_sorts/shell_sort.cpp)
target_link_libraries(shell_sort)

add_executable(sort chapter_4/1_sort/1_sort.cpp)
target_link_libraries(sort stix)

add_executable(quick_sort chapter_4/0_sorts/quick_sort.cpp)
target_link_libraries(quick_sort)

add_executable(external_sort
        chapter_4/2_external_sort/2_external_sort.cpp
        chapter_4/2_external_sort/working_filepaths.hpp
        chapter_4/2_external_sort/working_filepaths.cpp
        chapter_4/2_external_sort/read_to_files.hpp
        chapter_4/2_external_sort/read_to_files.cpp chapter_4/2_external_sort/merge_files.cpp chapter_4/2_external_sort/merge_files.hpp)
target_link_libraries(external_sort stix)
add_executable(external_sort_test chapter_4/2_external_sort/test.cpp chapter_4/2_external_sort/working_filepaths.hpp chapter_4/2_external_sort/working_filepaths.cpp)
target_link_libraries(external_sort_test stix testsupport)

add_executable(unique
        chapter_4/3_unique/3_unique.cpp
        chapter_4/3_unique/unique.cpp
        chapter_4/3_unique/unique.hpp)
target_link_libraries(unique stix)
add_executable(unique_test
        chapter_4/3_unique/test.cpp
        chapter_4/3_unique/unique.cpp
        chapter_4/3_unique/unique.hpp)
target_link_libraries(unique_test stix testsupport)

add_executable(kwic
        chapter_4/4_kwic/4_kwic.cpp
        chapter_4/4_kwic/kwic.cpp
        chapter_4/4_kwic/kwic.hpp)
target_link_libraries(kwic stix)
add_executable(kwic_test
        chapter_4/4_kwic/test.cpp
        chapter_4/4_kwic/kwic.cpp
        chapter_4/4_kwic/kwic.hpp)
target_link_libraries(kwic_test stix testsupport)

add_executable(unrotate
        chapter_4/5_unrotate/5_unrotate.cpp chapter_4/5_unrotate/unrotate.hpp chapter_4/5_unrotate/unrotate.cpp)
target_link_libraries(unrotate stix)
add_executable(unrotate_test
        chapter_4/5_unrotate/test.cpp chapter_4/5_unrotate/unrotate.hpp chapter_4/5_unrotate/unrotate.cpp)
target_link_libraries(unrotate_test stix testsupport)

add_executable(find
        chapter_5/1_find/1_find.cpp
        lib/regex/matcher.hpp
        lib/regex/pattern_matcher.cpp
        lib/regex/char_seq.hpp
        lib/escapes.hpp
        chapter_5/1_find/find.cpp
        chapter_5/1_find/find.hpp)
target_link_libraries(find stix)
add_executable(find_test
        chapter_5/1_find/test.cpp
        lib/regex/matcher.hpp
        lib/regex/pattern_matcher.cpp
        lib/regex/char_seq.hpp
        lib/escapes.hpp
        chapter_5/1_find/find.cpp
        chapter_5/1_find/find.hpp)
target_link_libraries(find_test stix testsupport)

add_executable(find_cheating
        chapter_5/1_find_cheating/1_find_cheating.cpp)
target_link_libraries(find_cheating stix)

add_executable(change
        chapter_5/2_change/2_change.cpp)
target_link_libraries(change stix)

add_executable(change_test
        chapter_5/2_change/test.cpp)
target_link_libraries(change_test stix testsupport)

set (edit_sources
        chapter_6/1_edit/line_buffer.cpp
        chapter_6/1_edit/line_buffer.hpp
        chapter_6/1_edit/edit_buffer.cpp
        chapter_6/1_edit/edit_buffer.hpp
        chapter_6/1_edit/editor.cpp
        chapter_6/1_edit/editor.hpp
        chapter_6/1_edit/command_parser.cpp
        chapter_6/1_edit/command_parser.hpp
        chapter_6/1_edit/command_actions.cpp
        chapter_6/1_edit/command_actions.hpp
	chapter_6/1_edit/command.cpp 
	chapter_6/1_edit/command.hpp
		chapter_6/1_edit/commands.hpp
		chapter_6/1_edit/commands.cpp)
add_executable(edit
        chapter_6/1_edit/1_edit.cpp
        ${edit_sources})
target_link_libraries(edit stix)

add_executable(edit_test
        chapter_6/1_edit/test_line_buffer.cpp
        chapter_6/1_edit/test_edit_buffer.cpp
        chapter_6/1_edit/test_editor.cpp
        chapter_6/1_edit/test_command_parser.cpp
        ${edit_sources}
        chapter_6/1_edit/test_command_actions.cpp)
target_link_libraries(edit_test stix testsupport)

set (format_sources
		chapter_7/1_format/format.hpp
		chapter_7/1_format/formatter.hpp
		chapter_7/1_format/formatter.cpp
		chapter_7/1_format/format_functions.hpp
		chapter_7/1_format/format_functions.cpp
		chapter_7/1_format/filler.cpp
		chapter_7/1_format/filler.hpp
)
add_executable(format
		chapter_7/1_format/1_format.cpp
		${format_sources})
target_link_libraries(format stix)

add_executable(format_test
		chapter_7/1_format/test_format.cpp
		${format_sources}
		chapter_7/1_format/test_functions.cpp
)
target_link_libraries(format_test stix testsupport)

set (macro_sources
		chapter_8/1_macro/macro.hpp
		chapter_8/1_macro/macro.cpp
		chapter_8/1_macro/source/token_seq.hpp
		chapter_8/1_macro/source/token_seq.cpp
		chapter_8/1_macro/source/token_stream.hpp
		chapter_8/1_macro/source/token_stream.cpp
		chapter_8/1_macro/source/end_of_input.hpp
		chapter_8/1_macro/mp/support.cpp
		chapter_8/1_macro/mp/support.hpp
		chapter_8/1_macro/mp/predefined.hpp
		chapter_8/1_macro/source/expression_engine.cpp
		chapter_8/1_macro/source/expression_engine.hpp
)
add_executable(macro
		chapter_8/1_macro/1_macro.cpp
		${macro_sources}
)
target_link_libraries(macro stix)

add_executable(macro_test
		chapter_8/1_macro/test/test_macro.cpp
		chapter_8/1_macro/test/test_tokenizer.cpp
		${macro_sources}
		chapter_8/1_macro/test/test.hpp
		chapter_8/1_macro/test/test.cpp
		chapter_8/1_macro/test/test_ifelse.cpp
		chapter_8/1_macro/test/test_len.cpp
		chapter_8/1_macro/test/test_quotes.cpp
		chapter_8/1_macro/test/test_substr.cpp
		chapter_8/1_macro/test/test_expr.cpp
		chapter_8/1_macro/test/test_expression_engine.cpp
)
target_link_libraries(macro_test stix testsupport)

