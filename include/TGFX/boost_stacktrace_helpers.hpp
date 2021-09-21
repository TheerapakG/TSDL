#ifndef _TGFX_BOOST_STACKTRACE_HELPERS
#define _TGFX_BOOST_STACKTRACE_HELPERS

#include "TGFX/macros.hpp"

#include <ranges>

#include "boost/stacktrace/stacktrace.hpp"
#include "boost/stacktrace/frame.hpp"
#include "fmt/core.h"
#include "fmt/color.h"

TGFX_SUBSPACE_IMPL_BEGIN(boost_helpers)

std::string_view minimal_frame_format = "\033[1m{}:{}:\033[0m {}";
std::string_view minimal_addr_format = "{}";
std::string_view minimal_function_format = "{}";
std::string_view py_frame_format = "File {}, line {}, function {}";
std::string_view py_addr_format = "at {}";
std::string_view py_function_format = "function {}";

std::string_view frame_format = minimal_frame_format;
std::string_view addr_format = minimal_addr_format;
std::string_view function_format = minimal_function_format;

template <typename OutputIt, typename S, typename RangeAdapter>
inline auto format_stacktrace_to(
  OutputIt out, 
  const S& traceback_header, 
  const boost::stacktrace::stacktrace& trace,
  const RangeAdapter& trace_selector
) {
  fmt::format_to(out, "\033[1m{}\033[0m", traceback_header);

  if (trace.empty()) fmt::format_to(out, "\n    \033[31mCannot capture stacktrace\033[0m");
  else {
    for (boost::stacktrace::frame frame: trace | trace_selector) {
      std::size_t src_line = frame.source_line();
      std::string f_name = frame.name();
      fmt::format_to(out, "\n    ");
      if (src_line==0) {
        fmt::format_to(
          out, 
          frame_format, 
          "<unknown>", "<unknown>",
          fmt::format(f_name.empty() ? addr_format : function_format, to_string(frame))
        );
      }
      else fmt::format_to(
        out, 
        frame_format, 
        frame.source_file(), 
        src_line, 
        f_name
      );
    }
  }
  return out;
}

template <typename OutputIt, typename S>
inline auto format_stacktrace_to(
  OutputIt out, 
  const S& traceback_header, 
  const boost::stacktrace::stacktrace& trace
) -> decltype(fmt::format_to(out, "")) {
  return format_stacktrace_to(out, traceback_header, trace, std::ranges::views::all);
}

TGFX_SUBSPACE_IMPL_END

#endif
