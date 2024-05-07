#include "command_actions.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include "../../lib/regex/replacement.hpp"
#include "../../lib/regex/change.hpp"
#include "lines.hpp"
#include <fstream>
#include <sstream>

#include "command_parser.hpp"

using namespace stiX;

constexpr auto eof = std::char_traits<char>::eof();

namespace {
  void read_lines(
      std::istream& source,
      size_t index,
      bool const stop_on_stop,
      lines_modifier& buffer) {
    while(source.peek() != eof) {
      auto line = stiX::getline(source);

      if (stop_on_stop && line == ".")
        return;

      buffer.insert(index, line);
      ++index;
    }
  }

  void write_lines(std::ostream& out, size_t const from, size_t const to, lines_modifier const& buffer) {
    for (auto index = from; index <= to; ++index)
      out << buffer.line_at(index) << '\n';
  }

  void reverse(size_t const from, size_t const to, lines_modifier& buffer) {
    auto const m = ((to - from) / 2) + 1;
    for (auto i = 0; i != m; ++i)
      buffer.swap(from + i, to - i);
  }

  std::vector<std::pair<size_t, size_t>> move_pairs(size_t from, size_t to, size_t after) {
    auto const offset = to - from;

    if (from < after)
      return std::vector<std::pair<size_t, size_t>> {
            {from, after},
            {after-offset, after},
            {from, after-(offset+1)}
      };

    return std::vector<std::pair<size_t, size_t>> {
        {after+1, to},
        {after+1, after+offset+1},
        {after+offset+2, to}
    };
  }
}

void stiX::line_index_action(std::ostream& out, size_t const to) {
  out << to << '\n';
}

void stiX::append_action(std::istream& in, size_t const after, lines_modifier& buffer) {
  read_lines(in, after, true, buffer);
}

void stiX::insert_action(std::istream& in, size_t const before, lines_modifier& buffer) {
  auto const adjust = !buffer.empty() ? 1 : 0;
  read_lines(in, before-adjust, true, buffer);
}

void stiX::change_action(std::istream& in, size_t const from, size_t const to, lines_modifier& buffer) {
  delete_action(from, to, buffer);
  insert_action(in, !buffer.empty() ? from : 0, buffer);
}

void stiX::move_action(size_t const from, size_t const to, size_t const after, lines_modifier& buffer) {
  auto const moves = move_pairs(from, to, after);
  for (auto [b, e] : moves)
    reverse(b, e, buffer);
}

void stiX::delete_action(size_t const from, size_t const to, lines_modifier& buffer) {
  for (auto i = to; i >= from; --i)
    buffer.remove_at(from);
}

void stiX::print_action(std::ostream& out, size_t const from, size_t const to, lines_modifier& buffer) {
  if (buffer.empty()) {
    out << "?\n";
    return;
  }

  buffer.set_dot(to);
  write_lines(out, from, to, buffer);
}

void stiX::filename_action(std::string_view const filename, std::string& property, std::ostream& out) {
  if (!filename.empty())
    property = filename;

  out << (!property.empty() ? property : "?") << "\n";
}

void stiX::write_to_file_action(size_t from, size_t to, std::string_view const filename, std::string& property, lines_modifier& buffer) {
  if (!filename.empty())
    property = filename;

  auto destination = std::ofstream(property);
  write_lines(destination, from, to, buffer);
}

void stiX::read_from_file_action(size_t before, std::string_view filename, std::string& property, lines_modifier& buffer) {
  if (!filename.empty())
    property = filename;

  auto source = std::ifstream(property);
  read_lines(source, before, false, buffer);
}

void stiX::edit_file_action(std::string_view filename, std::string& property, lines_modifier& buffer) {
  if (!filename.empty())
    property = filename;

  if (!buffer.empty())
    delete_action(1, buffer.last(), buffer);
  read_from_file_action(0, filename, property, buffer);
}

void stiX::substitute_action(
  size_t from,
  size_t to,
  std::string_view pattern,
  std::string_view replacement,
  bool replace_all,
  lines_modifier& buffer) {

  auto matcher = compile_pattern(pattern);
  auto replacer = prepare_replacement(replacement);

  auto change_fn = replace_all ? apply_change : apply_change_once;

  for(auto i = from; i <= to; ++i) {
    auto l = buffer.line_at(i);
    auto o = std::ostringstream { };

    change_fn(matcher, replacer, l, o);

    buffer.set_dot(i);
    buffer.set_at(i, o.str());
  }
}

void stiX::error_action(std::istream&, std::ostream& out, lines_modifier&, std::string&) {
  out << "?\n";
}

////////////////////
action_fn stiX::make_append_action(size_t const, size_t const to_index, size_t const, command_extras const&) {
  return [to_index](std::istream& in, std::ostream&, lines_modifier& buffer, std::string&) {
    append_action(in, to_index, buffer);
  };
}
action_fn stiX::make_change_action(size_t const from_index, size_t const to_index, size_t const, command_extras const&) {
  return [from_index, to_index](std::istream& in, std::ostream&, lines_modifier& buffer, std::string&) {
    change_action(in, from_index, to_index, buffer);
  };
}
action_fn stiX::make_delete_action(size_t const from_index, size_t const to_index, size_t const, command_extras const&) {
  return [from_index, to_index](std::istream&, std::ostream&, lines_modifier& buffer, std::string&) {
    delete_action(from_index, to_index, buffer);
  };
}
action_fn stiX::make_edit_action(size_t const, size_t const, size_t const, command_extras const& extras) {
  auto new_filename = extras.filename;
  return [new_filename](std::istream&, std::ostream&, lines_modifier& buffer, std::string& filename) {
    edit_file_action(new_filename, filename, buffer);
  };
}
action_fn stiX::make_filename_action(size_t const, size_t const, size_t const, command_extras const& extras) {
  auto new_filename = extras.filename;
  return [new_filename](std::istream& in, std::ostream& out, lines_modifier&, std::string& filename) {
    filename_action(new_filename, filename,  out);
  };
}
action_fn stiX::make_insert_action(size_t const, size_t const to_index, size_t const, command_extras const&) {
  return [to_index](std::istream& in, std::ostream&, lines_modifier& buffer, std::string&) {
    insert_action(in, to_index, buffer);
  };
}
action_fn stiX::make_move_action(size_t const from_index, size_t const to_index, size_t const destination,
  command_extras const&) {
  return [from_index, to_index, destination](std::istream&, std::ostream&, lines_modifier& buffer, std::string&) {
    move_action(from_index, to_index, destination, buffer);
  };
}
action_fn stiX::make_print_action(size_t const from_index, size_t const to_index, size_t const, command_extras const&) {
  return [from_index, to_index](std::istream&, std::ostream& out, lines_modifier& buffer, std::string&) {
    print_action(out, from_index, to_index, buffer);
  };
}
action_fn stiX::make_print_next_line_action(size_t const, size_t const to_index, size_t const, command_extras const&) {
  return [to_index](std::istream&, std::ostream& out, lines_modifier& buffer, std::string&) {
    print_action(out, to_index, to_index, buffer);
  };
}
action_fn stiX::make_quit_action(size_t const, size_t const, size_t const, command_extras const&) {
  return [](std::istream&, std::ostream&, lines_modifier&, std::string&) {
    std::exit(0);
  };
}
action_fn stiX::make_read_file_action(size_t const, size_t const to_index, size_t const, command_extras const& extras) {
  auto new_filename = extras.filename;
  return [to_index, new_filename](std::istream&, std::ostream&, lines_modifier& buffer, std::string& filename) {
    read_from_file_action(to_index, new_filename, filename, buffer);
  };
}
action_fn stiX::make_substitute_action(size_t const from_index, size_t const to_index, size_t const, command_extras const& extras) {
  auto pattern = extras.search_pattern;
  auto replacement = extras.replacement;
  auto replace_all = extras.replace_all;
  return [from_index, to_index, pattern, replacement,replace_all](std::istream&, std::ostream&, lines_modifier& buffer, std::string& filename) {
    substitute_action(from_index, to_index, pattern, replacement, replace_all, buffer);
  };
}
action_fn stiX::make_write_file_action(size_t const from_index, size_t const to_index, size_t const, command_extras const& extras) {
  auto new_filename = extras.filename;
  return [from_index, to_index, new_filename](std::istream&, std::ostream&, lines_modifier& buffer, std::string& filename) {
    write_to_file_action(from_index, to_index,new_filename, filename, buffer);
  };
}
action_fn stiX::make_line_index_action(size_t const, size_t const to_index, size_t const, command_extras const&) {
  return [to_index](std::istream&, std::ostream& out, lines_modifier&, std::string&) {
    line_index_action(out, to_index);
  };
}