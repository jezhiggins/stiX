//
// `edit` edit text files
//
// `edit [file]`
//
// `edit` is an interactive text editor that reads command lines from its input
// and writes display information, upon command, to its output. It works by
// reading text files on command into an internal "buffer" (which may be quite
// large), displaying and modifying the buffer contents by other commands, then
// writing all or part of the buffer to text files, also on command. The buffer
// is organised as a sequence of lines, numbered from 1; lines are implicitly
// renumbered as text is added or deleted.
//
// Context searches and substitutions are specified by writing text patterns,
// following the same rules for building patterns as used by find.
// Substitutions specify replacement text following the same rules as used by
// the program change.
//
// Line numbers are formed by the following components:
//
// `n` a decimal number
// `.` the current line ("dot")
// `$` the last line
// `/pattern/` a forward context search
// `\pattern\` a backward context search
//
// Components may be combined with `+` or `-`, as in, for example
//
// `.+1` sum of `.` and `1`
// `$-5` five lines before `$`
//
// Line numbers are separated by commas or semicolons; a semicolon sets the
// current line to the most recent line number preceding.
//
// Commands may be preceded by an arbitrary number of lines numbers (except
// for `e`, `f` and `q`, which require none be present). The last one or two
// are used as needed. If two line numbers are needed and only one is
// specified, it is used for both. If no line numbers are specified, a default
// rule is applied:
//
// `(.)` use the current line
// `(.+1)` use the next line
// `(.,.)` use the current line for both line numbers
// `(1,$)` use all lines
//   In alphabetical order, the command and their default line numbers are
// `(.) a` append text after line (text follows)
// `(.,.) c` change text (text follows)
// `(.,.) dp` delete text
// `e _file_` edit _file_ after discarding all previous text, remember file
//    name
// `f _file_` print file name, remember file name
// `(.) i` insert text before line (text follows)
// `(.,.) m _line3_ p` move text to after _line3_
// `(.,.) p` print text
// `q` quit
// `(.) r _file_` read _file_, appending after line
// `(.,.) s/_pat_/_new_/gp` substitute _new_ for occurrence of _pat_ (_g_
//    implies for each occurrence across line)
// `(1,$) w _file_` write _file_ (leaves current state unaltered)
// `(.) =p` print line number
// `(.,+1) _newline_` print one line
//
// The trailing `p`, which is optional, causes the last affected line to be
// printed. Dot is set to the last affected line, except for `f`, `w`, and `=`,
// for which it is unchanged.
//
// Text entered with `a`, `c` and `i` is terminated with a line containing just
// a `.`.
//
// The global prefixes cause repeated execution of a command, one for each line
// that matches (`g`) or does not match (`x`) a specified text pattern:
//
// `(1,$) g/_pattern_/_command_`
// `(1,$) x/_pattern_/_command_`
//
// `command` can be anything but `a`, `c`, `i` or `q`, and may be preceded by
// line numbers as usual. Dot is set to the matched line before _command_ is
// done.
//
// If the command line argument `file` is present, then the editor behaves as
// if its input began with the command `e file`. The first filename used is
// remembered, so that a subsequent `e`, `f`, `r`, or `w` command can be
// written with no filename to refer to the remembered filename. A filename
// given with `e` or `f` replaces any remembered filename.

#include "editor.hpp"
#include <iostream>

int main(int argc, char const* argv[]) {
  auto editor = stiX::editor();

  if (argc > 1)
    editor.load(argv[1], std::cout);

  editor.process(std::cin, std::cout);
}
