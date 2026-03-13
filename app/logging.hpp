#pragma once

inline void log_info(std::string_view str)
{
    fmt::println(stdout, "{}", str);
}

inline void log_error(std::string_view str)
{
    fmt::println(stderr, "{}", str);
}

template <typename... Args>
void log_info(fmt::format_string<Args...> fmtstr, Args&&... args)
{
    fmt::println(stdout, fmtstr, std::forward<Args>(args)...);
}

template <typename... Args>
void log_error(fmt::format_string<Args...> fmtstr, Args&&... args)
{
    fmt::println(stderr, fmtstr, std::forward<Args>(args)...);
}

