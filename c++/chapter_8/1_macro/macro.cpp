#include "macro.hpp"
#include "source/token_stream.hpp"
#include "source/expression_engine.hpp"
#include "mp/support.hpp"
#include "mp/predefined.hpp"
#include "../../lib/chars.hpp"

#include <functional>
#include <map>
#include <sstream>
#include <ranges>
#include <algorithm>

namespace {
  using namespace std::string_literals;
  using stiX::token_seq;
  using stiX::token_stream;
  using macro_arguments = std::vector<stiX::token_seq>;

  auto const Empty = ""s;
  auto const EmptyArguments = macro_arguments { };

  class macro_processor {
  public:
    void process(
      std::istream& in,
      std::ostream& out,
      std::ostream* warning = nullptr
    );

  private:
    using token_sink = std::function<void(std::string const&)>;
    using macro_fn = void (macro_processor::*)(
      std::string const&,
      macro_arguments const&,
      token_stream&,
      token_sink&
    );
    enum class macro_argument_type {
      required,
      optional,
      none
    };
    struct macro_def {
      macro_fn fn;
      macro_argument_type args;
      size_t min_args;
      size_t max_args;
    };

    void frame(token_stream&& source, std::ostream& out);
    void frame(token_stream&& source, token_seq& result);
    void frame(token_stream&& source, token_sink&& sink);
    std::string sub_frame_to_string(token_seq const& in);
    token_seq sub_frame_to_seq(token_seq const& in);

    auto all_to_(std::vector<token_seq> const& args, auto transform_fn) {
      return args
        | std::views::transform(transform_fn)
        | std::ranges::to<std::vector>();
    }
    std::vector<std::string> all_to_string(std::vector<token_seq> const& args) {
      return all_to_(args, [this](token_seq const& a) { return sub_frame_to_string(a); });
    }
    std::vector<token_seq> all_to_seq(std::vector<token_seq> const& args) {
      return all_to_(args, [this](token_seq const& a) { return sub_frame_to_seq(a); });
    }

    void define_replacement(std::string const&, macro_arguments const&, token_stream&, token_sink&);
    void len_macro(std::string const&, macro_arguments const&, token_stream&, token_sink&);
    void ifelse_macro(std::string const&, macro_arguments const&, token_stream&, token_sink&);
    void expr_macro(std::string const&, macro_arguments const&, token_stream&, token_sink&);
    void substr_macro(std::string const&, macro_arguments const&, token_stream&, token_sink&);
    void changeq_macro(std::string const&, macro_arguments const&, token_stream&, token_sink&);
    void install_quotes(std::string_view open, std::string_view close);
    void quoted_sequence(std::string const&, macro_arguments const&, token_stream&, token_sink&);
    void apply_replacement(std::string const&, macro_arguments const&, token_stream&, token_sink&);

    void install_macro(
      std::string_view name,
      macro_fn fn,
      macro_argument_type arg_type,
      size_t min_args = 0,
      size_t max_args = std::numeric_limits<size_t>::max()
    ) {
      macros_[std::string(name)] = {
        fn,
        arg_type,
        min_args,
        max_args
      };
    }
    bool is_macro(std::string const& tok) const {
      return macros_.contains(tok);
    }
    macro_def const& macro_definition(std::string const& tok) {
      return macros_[tok];
    }
    void invoke_macro(std::string const&, token_stream&, token_sink&);

    void set_warning_out(std::ostream* warning) {
      warning_ = warning;
    }
    void warning_if_excess(
        std::string const& tok,
        std::vector<token_seq> const& args,
        size_t max_args) {
      if (args.size() > max_args)
        warning(std::format("excess arguments to `{}' ignored", tok));
    }
    void warning_if_too_few(
      std::string const& tok,
      std::vector<token_seq> const& args,
      size_t min_args) {
      if (args.size() < min_args)
        warning(std::format("too few arguments to `{}'", tok));
    }
    void warning(std::string&& w) {
      if (warning_)
        *warning_ << "Warning: " << w << '\n';
    }

    std::map<std::string, macro_def> macros_;
    std::map<std::string, token_seq> replacements_;
    std::string open_quote;
    std::string close_quote;
    std::ostream* warning_ = nullptr;
  };

  using namespace mp;
  using namespace pre;

  /////////////////////
  void macro_processor::process(
    std::istream& in,
    std::ostream& out,
    std::ostream* warning
  ) {
    set_warning_out(warning);

    install_macro(Define, &macro_processor::define_replacement, macro_argument_type::required, 0, 2);
    install_macro(Len, &macro_processor::len_macro, macro_argument_type::required, 0, 1);
    install_macro(IfElse, &macro_processor::ifelse_macro, macro_argument_type::required, 0, 4);
    install_macro(Expr, &macro_processor::expr_macro, macro_argument_type::required, 1, 1);
    install_macro(Substr, &macro_processor::substr_macro, macro_argument_type::required, 2, 3);
    install_macro(ChangeQ, &macro_processor::changeq_macro, macro_argument_type::required, 0, 1);
    install_quotes(Grave, Apostrophe);

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
    token_sink&& sink)
  {
    while(source.token_available()) {
      auto token = source.pop_token();

      if (is_macro(token))
        invoke_macro(token, source, sink);
      else
        sink(token);
    }
  } // process

  std::string macro_processor::sub_frame_to_string(token_seq const& in) {
    auto sink = std::ostringstream { };
    frame(token_stream { in }, sink);
    return sink.str();
  }

  token_seq macro_processor::sub_frame_to_seq(token_seq const& in) {
    auto sink = token_seq { };
    frame(token_stream { in }, sink);
    return sink;
  }

  void macro_processor::define_replacement(
      std::string const& macro,
      macro_arguments const& raw_arguments,
      token_stream& source,
      token_sink& sink
  ) {
    auto def = !raw_arguments.empty()
      ? sub_frame_to_string(raw_arguments[0])
      : Empty;
    if (!stiX::isalnum(def))
      return;

    auto replacement = raw_arguments.size() >= 2
      ? sub_frame_to_seq(raw_arguments[1])
      : token_seq { };

    replacements_[def] = replacement;
    install_macro(def, &macro_processor::apply_replacement, macro_argument_type::optional);
  }

  void macro_processor::len_macro(
    std::string const& macro,
    macro_arguments const& raw_arguments,
    token_stream& source,
    token_sink& sink
  ) {
    auto expansion = !raw_arguments.empty()
      ? sub_frame_to_string(raw_arguments[0])
      : Empty;

    source.push_token(std::to_string(expansion.size()));
  }

  void macro_processor::ifelse_macro(
    std::string const& macro,
    macro_arguments const& raw_arguments,
    token_stream& source,
    token_sink& sink
  ) {
    if (raw_arguments.size() < 3)
      return;

    auto arguments = all_to_string(raw_arguments);

    auto const& lhs = arguments[0];
    auto const& rhs = arguments[1];
    auto const& then_r = arguments[2];
    auto const& else_r = arguments.size() >= 4 ? arguments[3] : Empty;

    auto const& result = lhs == rhs ? then_r : else_r;
    source.push_token(result);
  }

  void macro_processor::expr_macro(
    std::string const& macro,
    macro_arguments const& raw_arguments,
    token_stream& source,
    token_sink& sink
  ) {
    if (raw_arguments.empty()) {
      sink("0");
      return;
    }

    auto expr = sub_frame_to_seq(raw_arguments[0]);
    auto [val, val_ok] = evaluate_expression(expr);
    if (val_ok)
      sink(std::to_string(val));
    else
      warning(std::format("bad expression in `{}': {}", macro, expr.front()));
  }

  void macro_processor::substr_macro(
    std::string const& macro,
    macro_arguments const& raw_arguments,
    token_stream& source,
    token_sink& sink
  ) {
    auto arguments = all_to_seq(raw_arguments);
    auto const& str = !arguments.empty() ? sub_frame_to_string(arguments[0]) : Empty;
    auto [start, start_ok] = eval_int_arg(arguments, 1);
    auto [len, len_ok] = eval_int_arg(arguments, 2, static_cast<int>(std::string::npos));

    if (!start_ok || !len_ok) {
      warning(std::format("non-numeric argument to `{}'", macro));
      return;
    }

    if (start < str.size())
      source.push_token(str.substr(start, len));
  }

  void macro_processor::changeq_macro(
    std::string const& macro,
    macro_arguments const& raw_arguments,
    token_stream& source,
    token_sink& sink
  ) {
    auto arguments = all_to_string(raw_arguments);
    if (arguments.empty()) {
      install_quotes(Grave, Apostrophe);
      return;
    }

    auto const& quotes = arguments[0];
    auto end = quotes.length() > 1 ? 1 : 0;
    install_quotes(quotes.substr(0, 1), quotes.substr(end, 1));
  }

  void macro_processor::install_quotes(std::string_view open, std::string_view close) {
    macros_.erase(open_quote);
    open_quote = open;
    close_quote = close;
    install_macro(open_quote, &macro_processor::quoted_sequence, macro_argument_type::none);
  }

  void macro_processor::quoted_sequence(
    std::string const& token,
    macro_arguments const&,
    token_stream& source,
    token_sink& sink
  ) {
    source.push_token(token);
    auto quoted_seq = bracketed_sequence(source, open_quote, close_quote);
    drop_bracketing(quoted_seq);
    std::ranges::for_each(quoted_seq, [&sink](std::string const& t) { sink(t); });
  }

  void macro_processor::apply_replacement(
    std::string const& token,
    macro_arguments const& raw_arguments,
    token_stream& source,
    token_sink&
  ) {
    auto arguments = all_to_seq(raw_arguments);

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

  void macro_processor::invoke_macro(
    std::string const& macro,
    token_stream& source,
    token_sink& sink) {
    auto& def = macro_definition(macro);

    if (def.args == macro_argument_type::required && do_not_evaluate(macro, source, sink))
      return;
    auto const raw_arguments =
      def.args != macro_argument_type::none
      ? gather_arguments(source)
      : EmptyArguments;

    warning_if_excess(macro, raw_arguments, def.max_args);
    warning_if_too_few(macro, raw_arguments, def.min_args);

    std::invoke(def.fn, this, macro, raw_arguments, source, sink);
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
  do_macro_process(in, out, nullptr);
}

void stiX::macro_process(
  std::istream& in,
  std::ostream& out,
  std::ostream& err
) {
  do_macro_process(in, out, &err);
}