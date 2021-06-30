//
// find   find patterns in text
//
// find pattern
//
// find treads its input a line at a time and writes to its output
// those lines that match the specified text pattern. A text
// pattern is a concatenation of the following elements
//   c        literal character c
//   ?        any character except newline
//   %        beginning of the line
//   $        end of the line (null string before newline)
//   [...]    character class (any of these characters)
//   [^...]   negated character class (all but these characters)
//   *        closure (zero or more occurrences of previous pattern)
//   @c       escaped character (eg @%, @[, @*)
// Special meaning of characters in a text pattern is lost when
// escaped, inside [...] (except @]), or for:
//   %        not at beginning
//   $        not at end
//   *        at beginning
// A character class consists of zero of more of the following
// elements, surrounded by [ and ]:
//   c        literal character c, including [
//   c1-c2    range of characters (digits, lower or upper case letters)
//   ^        negated character class if at beginning
//   @c       escaped character (eg @^, @-,, @@ @])
// Special meaning of characters in a character class is lost when
// escaped or for:
//   ^        not at beginning
//   -        at beginning or end
// An escape sequence consists of the character @ followed by a
// single character:
//   @n       newline
//   @t       tab
//   @c       c (including @@)
//
// To print line ending in a Pascal keyword or identifier:
//   find [a-zA-Z][a-zA-Z0-9]*$
//

int main() {
}