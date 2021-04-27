//
// unrotate  format lines for KWIC index
//
// unrotate reads its input a line at a time and writes an
// "unfolded" version to its output. A lines is "folded" if
// it contains within it an instance of the fold character
// $; "unfolding" involves writing from the end of the line
// down to but not including the old character, starting in
// column 39 of the output line, wrapping characters that
// would thes appear before column 1 around to the end of
// the line, then writing the remainder of the line
// starting at column 41 and wrapping around at column 80
// if necessary.
// unrotate is used with kwic and sort to produce a KeyWord
// In Context, or KWIC, index.
//
// unrotate
// a test.$This is
// is a test.$This
// test.$This is a
// This is a test.$
// <ENDFILE>
//                            This is  a test.
//                               This  is a test.
//                          This is a  test.
//                                     This is a test.
//
int main() {

}