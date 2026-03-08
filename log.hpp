#pragma once

#include <format>
#include <string_view>
#include <type_traits>
#include <tuple>
#include <utility>
#include <source_location>

namespace loglib
{
    // Implemented in log.cpp
    void log_impl(std::source_location loc, std::string_view fmt, std::format_args args);

    // Helper: create `std::format_args` from forwarded arguments by
    // storing decayed copies in a tuple so we can pass lvalue references
    // to `std::make_format_args` (it requires lvalues).
    template <typename... A>
    inline std::format_args make_format_args_for(A &&...a)
    {
        if constexpr ((std::is_lvalue_reference_v<A> && ...))
        {
            return std::make_format_args(std::forward<A>(a)...);
        }
        else
        {
            auto copies = std::tuple<std::decay_t<A>...>(std::forward<A>(a)...);
            return std::apply([](auto &...items)
                              { return std::make_format_args(items...); }, copies);
        }
    }

    // Single wrapper: if `fmt` is convertible to `std::format_string<Args...>`
    // then the call is compile-time checked; otherwise we accept a
    // dynamic `std::string_view` format and fall back to runtime checks.
    template <typename Fmt, typename... Args>
    inline void log(std::source_location loc, Fmt &&fmt, Args &&...args)
    {
        if constexpr (
            std::is_convertible_v<Fmt, std::format_string<Args...>>)
        {
            log_impl(
                loc,
                std::forward<Fmt>(fmt),
                make_format_args_for(std::forward<Args>(args)...));
        }
        else
        {
            log_impl(
                loc,
                std::string_view(std::forward<Fmt>(fmt)),
                make_format_args_for(std::forward<Args>(args)...));
        }
    }
}

// Macro front-end
// Accept zero or more format arguments. The `##__VA_ARGS__` removes the
// trailing comma when no additional args are passed (works with clang/gcc).
#define LOG(fmt, ...) \
    ::loglib::log(std::source_location::current(), fmt, ##__VA_ARGS__)
