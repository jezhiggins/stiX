#include "macro.hpp"
#include "source/token_stream.hpp"
#include "mp/support.hpp"
#include "mp/predefined.hpp"

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
      std::ostream& out
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
    std::pair<std::string, token_seq> name_and_replacement(token_stream& source);

    void len_macro(std::string const&, token_stream&, token_sink);
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

    std::map<std::string, macro_fn> macros_;
    std::map<std::string, token_seq> replacements_;
  };

  using namespace mp;
  using namespace pre;

  /////////////////////
  void macro_processor::process(
    std::istream& in,
    std::ostream& out
  ) {
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
      std::string const&,
      token_stream& source,
      token_sink
  ) {
    auto [ def, replacement ] = name_and_replacement(source);
    replacements_[def] = replacement;
    macros_[def] = &macro_processor::apply_replacement;
  }

  std::pair<std::string, token_seq> macro_processor::name_and_replacement(
    token_stream& source
  ) {
    check_next(source, LeftParen);

    auto define_source = mp::all_arguments(source);

    auto def = definition_name(define_source);
    auto replacement = sub_frame_to_seq(std::move(define_source));
    return { def, replacement };
  }

  void macro_processor::len_macro(
    std::string const&,
    token_stream& source,
    token_sink
  ) {
    auto expansion = sub_frame_to_string(
      all_arguments(source)
    );

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
} // namespace

void stiX::macro_process(
    std::istream& in,
    std::ostream& out
) {
  auto macro = macro_processor { };
  macro.process(in, out);
}