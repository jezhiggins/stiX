#include "command_actions.hpp"
#include "../../lib/getline.hpp"
#include "../../lib/regex/pattern_matcher.hpp"
#include "../../lib/regex/replacement.hpp"
#include "../../lib/regex/change.hpp"
#include "edit_buffer.hpp"
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
      edit_buffer& buffer) {
    while(source.peek() != eof) {
      auto line = stiX::getline(source);

      if (stop_on_stop && line == ".")
        return;

      buffer.insert(index, line);
      ++index;
    }
  }

  void write_lines(std::ostream& out, size_t const from, size_t const to, edit_buffer const& buffer) {
    for (auto index = from; index <= to; ++index)
      out << buffer.line_at(index) << '\n';
  }

  void reverse(size_t const from, size_t const to, edit_buffer& buffer) {
    auto const m = ((to - from) / 2) + 1;
    for (auto i = 0; i != m; ++i)
      buffer.swap(from + i, to - i);
  }

  std::vector<std::pair<size_t, size_t>> move_pairs(size_t from, size_t to, size_t after) {
    if (from == after + 1)
      return { };

    auto const offset = to - from;

    if (from < after)
      return {
            {from, after},
            {after-offset, after},
            {from, after-(offset+1)}
      };

    return {
        {after+1, to},
        {after+1, after+offset+1},
        {after+offset+2, to}
    };
  }

  void update_filename_property(std::string_view const filename, std::string& property) {
    if (!filename.empty())
      property = filename;
  }

  bool filename_is_good(std::string_view const property, std::ostream& out) {
    if (!property.empty())
      return true;
    error_action(out);
    return false;
  }

  size_t next_mark(size_t index, stiX::edit_buffer const& buffer) {
    while ((!buffer.mark(index)) && (index <= buffer.last()))
      ++index;
    return (index <= buffer.last()) ? index : -1;
  }

  void clear_marks(edit_buffer& buffer) {
    for (auto i = 1; i <= buffer.last(); ++i)
      buffer.clear_mark(i);
  }

  void mark_lines(size_t const from, size_t const to,
                  std::string_view pattern, bool match,
                  edit_buffer& buffer) {
    auto matcher = compile_pattern(pattern);

    for (auto i = from; i <= to; ++i) {
      auto l = buffer.line_at(i);

      if (matcher.match(l) == match)
        buffer.set_mark(i);
    }
  }

  void action_marked_lines(
    size_t const from,
    stiX::parsed_command const& action,
    std::istream& in,
    std::ostream& out,
    edit_buffer& buffer,
    std::string& filename)
  {
    for (auto i = next_mark(from, buffer); i != -1; i = next_mark(i, buffer)) {
      buffer.clear_mark(i);
      buffer.set_dot(i);

      auto command = action.compile(buffer);
      command(in, out, buffer, filename);

      if (command.is_error())
        return;
    }
  }
} // namespace

void stiX::line_index_action(std::ostream& out, size_t const to) {
  out << to << '\n';
}

void stiX::error_action(std::ostream& out) {
  out << "?\n";
}

void stiX::append_action(std::istream& in, size_t const after, edit_buffer& buffer) {
  read_lines(in, after, true, buffer);
}

void stiX::insert_action(std::istream& in, size_t const before, edit_buffer& buffer) {
  auto const adjust = (!buffer.empty() && before != 0) ? 1 : 0;
  read_lines(in, before-adjust, true, buffer);
}

void stiX::change_action(std::istream& in, size_t const from, size_t const to, edit_buffer& buffer) {
  delete_action(from, to, buffer);
  insert_action(in, !buffer.empty() ? from : 0, buffer);
}

void stiX::move_action(size_t const from, size_t const to, size_t const after, edit_buffer& buffer) {
  auto const moves = move_pairs(from, to, after);
  for (auto [b, e] : moves)
    reverse(b, e, buffer);
}

void stiX::delete_action(size_t const from, size_t const to, edit_buffer& buffer) {
  for (auto i = to; i >= from; --i)
    buffer.remove_at(from);
}

void stiX::print_action(std::ostream& out, size_t const from, size_t const to, edit_buffer& buffer) {
  buffer.set_dot(to);
  write_lines(out, from, to, buffer);
}

void stiX::filename_action(std::string_view const filename, std::string& property, std::ostream& out) {
  update_filename_property(filename, property);

  out << (!property.empty() ? property : "?") << "\n";
}

void stiX::write_to_file_action(std::ostream& out, size_t from, size_t to, std::string_view const filename, std::string& property, edit_buffer& buffer) {
  if (filename_is_good(property, out)) {
    auto destination = std::ofstream(property);
    write_lines(destination, from, to, buffer);
  }
}

void stiX::read_from_file_action(std::ostream& out, size_t before, std::string_view filename, std::string& property, edit_buffer& buffer) {
  if (filename_is_good(property, out)) {
    auto source = std::ifstream(property);
    read_lines(source, before, false, buffer);
  }
}

void stiX::edit_file_action(std::ostream& out, std::string_view filename, std::string& property, edit_buffer& buffer) {
  update_filename_property(filename, property);

  if (filename_is_good(property, out)) {
    if (!buffer.empty())
      delete_action(1, buffer.last(), buffer);
    read_from_file_action(out, 0, filename, property, buffer);
  }
}

void stiX::substitute_action(
  size_t from,
  size_t to,
  std::string_view pattern,
  std::string_view replacement,
  bool replace_all,
  edit_buffer& buffer) {

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

void stiX::global_action(
  size_t const from,
  size_t const to,
  std::string_view pattern,
  bool match,
  stiX::parsed_command const& action,
  std::istream& in,
  std::ostream& out,
  edit_buffer& buffer,
  std::string& filename)
{
  clear_marks(buffer);

  mark_lines(from, to, pattern, match, buffer);

  action_marked_lines(from, action, in, out, buffer, filename);
}

////////////////////
namespace {
  action from_is_good(size_t const from, action fn) {
    return from != 0 ? fn : stiX::command::error;
  }

  action make_global_action(
    size_t const from_index,
    size_t const to_index,
    command_extras const& extras,
    bool match) {
    if (from_index == 0)
      return stiX::command::error;

    auto pattern = extras.search_pattern;
    auto action = stiX::parse_command(extras.replacement);

    if (code_match(action.code, "acgiqx?"))
      return stiX::command::error;

    return [from_index, to_index, pattern, action, match](std::istream& in, std::ostream& out, edit_buffer& buffer, std::string& filename) {
      global_action(from_index, to_index, pattern, match, action, in, out, buffer, filename);
    };
  }
}

action stiX::make_append_action(size_t const, size_t const to_index, size_t const, command_extras const&) {
  return [to_index](std::istream& in, std::ostream&, edit_buffer& buffer, std::string&) {
    append_action(in, to_index, buffer);
  };
}

action stiX::make_change_action(size_t const from_index, size_t const to_index, size_t const, command_extras const&) {
  return from_is_good(from_index,
    [from_index, to_index](std::istream& in, std::ostream&, edit_buffer& buffer, std::string&) {
      change_action(in, from_index, to_index, buffer);
    }
  );
}
action stiX::make_delete_action(size_t const from_index, size_t const to_index, size_t const, command_extras const&) {
  return from_is_good(from_index,
    [from_index, to_index](std::istream&, std::ostream&, edit_buffer& buffer, std::string&) {
      delete_action(from_index, to_index, buffer);
    }
  );
}
action stiX::make_edit_action(size_t const, size_t const, size_t const, command_extras const& extras) {
  auto new_filename = extras.filename;
  return [new_filename](std::istream&, std::ostream& out, edit_buffer& buffer, std::string& filename) {
    edit_file_action(out, new_filename, filename, buffer);
  };
}
action stiX::make_filename_action(size_t const, size_t const, size_t const, command_extras const& extras) {
  auto new_filename = extras.filename;
  return [new_filename](std::istream& in, std::ostream& out, edit_buffer&, std::string& filename) {
    filename_action(new_filename, filename,  out);
  };
}

action stiX::make_global_match_action(size_t const from_index, size_t const to_index, size_t const, command_extras const& extras) {
  return make_global_action(from_index, to_index, extras, true);
}
action stiX::make_global_mismatch_action(size_t const from_index, size_t const to_index, size_t const, command_extras const& extras) {
  return make_global_action(from_index, to_index, extras, false);
}

action stiX::make_insert_action(size_t const, size_t const to_index, size_t const, command_extras const&) {
  return [to_index](std::istream& in, std::ostream&, edit_buffer& buffer, std::string&) {
    insert_action(in, to_index, buffer);
  };
}
action stiX::make_move_action(size_t const from_index, size_t const to_index, size_t const destination,
                              command_extras const&) {
  return [from_index, to_index, destination](std::istream&, std::ostream&, edit_buffer& buffer, std::string&) {
    move_action(from_index, to_index, destination, buffer);
  };
}
action stiX::make_print_action(size_t const from_index, size_t const to_index, size_t const, command_extras const&) {
  return from_is_good(from_index,
    [from_index, to_index](std::istream&, std::ostream& out, edit_buffer& buffer, std::string&) {
      print_action(out, from_index, to_index, buffer);
    }
  );
}
action stiX::make_print_next_line_action(size_t const, size_t const to_index, size_t const, command_extras const&) {
  return [to_index](std::istream&, std::ostream& out, edit_buffer& buffer, std::string&) {
    print_action(out, to_index, to_index, buffer);
  };
}
action stiX::make_quit_action(size_t const, size_t const, size_t const, command_extras const&) {
  return [](std::istream&, std::ostream&, edit_buffer&, std::string&) {
    std::exit(0);
  };
}
action stiX::make_read_file_action(size_t const, size_t const to_index, size_t const, command_extras const& extras) {
  auto new_filename = extras.filename;
  return [to_index, new_filename](std::istream&, std::ostream& out, edit_buffer& buffer, std::string& filename) {
    read_from_file_action(out, to_index, new_filename, filename, buffer);
  };
}
action stiX::make_substitute_action(size_t const from_index, size_t const to_index, size_t const, command_extras const& extras) {
  auto pattern = extras.search_pattern;
  auto replacement = extras.replacement;
  auto replace_all = extras.replace_all;
  return [from_index, to_index, pattern, replacement, replace_all](std::istream&, std::ostream&, edit_buffer& buffer, std::string& filename) {
    substitute_action(from_index, to_index, pattern, replacement, replace_all, buffer);
  };
}
action stiX::make_write_file_action(size_t const from_index, size_t const to_index, size_t const, command_extras const& extras) {
  auto new_filename = extras.filename;
  return from_is_good(from_index,
    [from_index, to_index, new_filename](std::istream&, std::ostream& out, edit_buffer& buffer, std::string& filename) {
      write_to_file_action(out, from_index, to_index, new_filename, filename, buffer);
    }
  );
}
action stiX::make_line_index_action(size_t const, size_t const to_index, size_t const, command_extras const&) {
  return [to_index](std::istream&, std::ostream& out, edit_buffer&, std::string&) {
    line_index_action(out, to_index);
  };
}