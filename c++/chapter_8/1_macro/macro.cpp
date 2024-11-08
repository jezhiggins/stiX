#include "macro.hpp"
#include "source/token_stream.hpp"
#include "mp/support.hpp"
#include "mp/predefined.hpp"
#include "../../lib/chars.hpp"

#include <functional>
#include <map>
#include <sstream>

namespace {
  using stiX::token_seq;
  using stiX::token_stream;

  class macro_processor {
  public:
    void process(
      std::istream& in,
      std::ostream& out,
      std::ostream* err = 0
    );

  private:
    using token_sink = std::function<void(std::string const&)>;
    using macro_fn = void (macro_processor::*)(
      std::string const&,
      token_stream&,
      token_sink
    );

    void frame(token_stream&& source, std::ostream& out);
    void frame(token_stream&& source, token_seq& result);
    void frame(token_stream&& source, token_sink sink);
    std::string sub_frame_to_string(token_stream&& in);
    token_seq sub_frame_to_seq(token_stream&& in);

    void define_replacement(std::string const&, token_stream&, token_sink);
    void len_macro(std::string const& macro, token_stream&, token_sink sink);
    void quoted_sequence(std::string const&, token_stream&, token_sink);
    void apply_replacement(std::string const&,token_stream&, token_sink);

    void install_macro(std::string_view name, macro_fn fn) {
      macros_[std::string(name)] = fn;
    }
    bool is_macro(std::string const& tok) const {
      return macros_.contains(tok);
    }
    macro_fn macro_function(std::string const& tok) {
      return macros_[tok];
    }

    void set_error(std::ostream* err) {
      err_ = err;
    }

    void warning(std::string const& w) {
      if (err_)
        *err_ << "Warning: " << w << '\n';
    }

    std::map<std::string, macro_fn> macros_;
    std::map<std::string, token_seq> replacements_;
    std::ostream* err_;
  };

  using namespace mp;
  using namespace pre;

  /////////////////////
  void macro_processor::process(
    std::istream& in,
    std::ostream& out,
    std::ostream* err
  ) {
    set_error(err);

    install_macro(Define, &macro_processor::define_replacement);
    install_macro(Len, &macro_processor::len_macro);
    install_macro(Grave, &macro_processor::quoted_sequence);

    frame(token_stream { in }, out);
  }

  void macro_processor::frame(
    token_stream&& source,
    std::ostream& out
  ) {
    frame(
      std::move(source),
      [&out](std::string const& token) { out << token; }
    );
  }

  void macro_processor::frame(
    token_stream&& source,
    token_seq& result
  ) {
    frame(
      std::move(source),
      [&result](std::string const& token) { result.push_back(token); }
    );
  }

  void macro_processor::frame(
    token_stream&& source,
    token_sink sink)
  {
    while(source.token_available()) {
      auto token = source.pop_token();

      if (is_macro(token)) {
        auto fn = macro_function(token);
        std::invoke(fn, this, token, source,sink);
      }
      else
        sink(token);
    }
  } // process

  std::string macro_processor::sub_frame_to_string(token_stream&& in) {
    auto sink = std::ostringstream { };
    frame(std::move(in), sink);
    return sink.str();
  }

  token_seq macro_processor::sub_frame_to_seq(token_stream&& in) {
    auto sink = token_seq { };
    frame(std::move(in), sink);
    return sink;
  }

  void macro_processor::define_replacement(
      std::string const& macro,
      token_stream& source,
      token_sink sink
  ) {
    if (!is_next(source, LeftParen)) {
      sink(macro);
      return;
    }

    auto const raw_arguments = gather_arguments(source);
    if (raw_arguments.empty())
      return;
    if (raw_arguments.size() > 2)
      warning("excess arguments to `define' ignored");

    auto def = sub_frame_to_string(token_stream { raw_arguments[0] });
    if (!stiX::isalnum(def)) {
      //throw std::runtime_error(std::format("{} is not alphanumeric", def));
      return;
    }
    auto replacement = raw_arguments.size() >= 2
                       ? sub_frame_to_seq(token_stream { raw_arguments[1] })
                       : token_seq { };

    replacements_[def] = replacement;
    macros_[def] = &macro_processor::apply_replacement;
  }

  void macro_processor::len_macro(
    std::string const& macro,
    token_stream& source,
    token_sink sink
  ) {
    if (!is_next(source, LeftParen)) {
      sink(macro);
      return;
    }

    auto const raw_arguments = gather_arguments(source);
    if (raw_arguments.size() > 2)
      warning("excess arguments to `len' ignored");

    auto expansion = !raw_arguments.empty()
      ? sub_frame_to_string(token_stream { raw_arguments[0] })
      : std::string { };

    source.push_token(std::to_string(expansion.size()));
  }

  void macro_processor::quoted_sequence(
    std::string const&,
    token_stream& source,
    token_sink sink
  ) {
    while(not_reached(source, Apostrophe))
      sink(source.pop_token());
    source.pop_token();
  }

  void macro_processor::apply_replacement(
    std::string const& token,
    token_stream& source,
    token_sink
  ) {
    auto const raw_arguments = gather_arguments(source);
    auto arguments = std::vector<token_seq> { };
    for (auto const& arg: raw_arguments)
      arguments.push_back(sub_frame_to_seq(token_stream(arg)));
    auto definition = token_stream { replacements_[token] };

    auto with_arg_substitution = token_seq { };
    while (definition.token_available()) {
      while (not_reached(definition, Dollar))
        with_arg_substitution += definition.pop_token();

      if (is_next(definition, Dollar))
        with_arg_substitution += argument_substitution(definition, arguments);
    }

    source.push_tokens(with_arg_substitution);
  }

  void do_macro_process(
    std::istream& in,
    std::ostream& out,
    std::ostream* err
  ) {
    auto macro = macro_processor { };
    macro.process(in, out, err);
  }
} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  do_macro_process(in, out, 0);
}

void stiX::macro_process(
  std::istream& in,
  std::ostream& out,
  std::ostream& err
) {
  do_macro_process(in, out, &err);
}