//
// `macro`   expand string definitions, with arguments
//
// `macro`
//
// `macro` reads its input, looking for macro definitions of the form
//      define(ident, string)
// and writes its output with each subsequent instance of the identifier
// `ident` replaced by the arbitrary sequence of characters `string`
// Within a replacement string, any dollar sign $ followed by a digit is
// replaced by an argument corresponding to that digit. Arguments are
// written as a parenthesized list of strings following an instance of
// the identifier, e.g.
//      ident(arg1, arg2, ...)
// So $1 is replaced in the arguments string by arg1, $2 by arg2, and so
// on; $0 is replaced by `ident`. Missing arguments are taken as null
// strings; extra arguments are ignored.
//
// The replacement string in a definition is expanded before the definition
// occurs, except that any sequence of characters between a grave ` and a
// balancing apostrophe ' is taken literally, with the grave and apostrophe
// removed. Thus, it is possible to make an alias for a define by writing
//     define(def, `define($1,$2)')
//
// Additional predefined built-ins are:
//
// `ifelse(a,b,c,d)` is replaced by the string `c` if the string `a` exactly
// matches the string `b`; otherwise it is replaced by the string `d`
//
// `expr(expression)` is replaced by the decimal string representation of
// the numeric value of `expression`. For correct operation, the expression
// must consist of parentheses, integer operands written as decimal digit
// strings, and the operators +, -, *, / (integer division), and % (remainder).
// Multiplication and division bind tighter than addition and subtraction, but
// parentheses may be used to alter this order.
//
// `substr(s,m,n)` is replaced by the substring of `s` starting at location
// `m` (counting from one) and continuing at most `n` characters. If `n` is
// omitted, it is taken as a very large number; if `n` is outside the string,
// the replacement string is null. `m` and `n` may be expressions suitable for
// `expr`.
//
// `len(s)` is replaced by the string representing the length of its argument
// in characters.
//
// `changeq(xy)` changes the quote character to `x` and `y`. `changeq()`
// changes them back to ` and ' respectively.
//
// Each replacement string is rescanned for further possible replacements,
// permitting multi-level definitions to be expanded to final form.
//

int main() {

}