//
// archive
//
// archive -cmd aname [ file ... ]
//
// archive manages any number of member files in a single
// file, aname, with sufficient information that members
// may be selectively added, extracted, replaced, or
// deleted from the collection. -cmd is a code that
// determines the operation to be performed.
//
//  -c  create a new archive with named members
//  -d  deleted named members from archive
//  -p  print named members on standard output
//  -t  print table of archive contents
//  -u  update named members or add at end
//  -x  extract named members from archive
//
// In each case, the "named members" are the zero or more
// filenames given as arguments following aname. If no
// arguments follow, then the "named members" are taken as
// all of the files in the archive, except for the delete
// command -d, which is not so rash. archive complains if a
// file is named twice or cannot be accessed.
// The -t command writes one line to the output for each
// named member, consisting of the member name and a string
// representation of the file length, separated by a blank.
// The create command -c makes a new archive containing the
// named files. The update command -u replaces existing named
// members and adds new files onto the end of an existing
// archive. Create and update read from, and extract writes,
// files whose names are the same as the member names in the
// archive. An intermediate version of the new file is first
// written to the file artemp; hence this filename should be
// avoided.
// An archive is a concatenation of zero of more entires,
// each consisting of a header and an exact copy of the
// original file. The header format is
//  -h- name length

#include "../../lib/arguments.hpp"
#include "./archive.hpp"

int main(int argc, const char* argv[]) {
  auto arguments = stiX::make_arguments(argc, argv);
  stiX::archive(arguments);
} // main