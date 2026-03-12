#pragma once

inline void log_info(std::string_view str)
{
    std::println(stdout, "{}", str);
}

inline void log_error(std::string_view str)
{
    std::println(stderr, "{}", str);
}

template <typename... Args>
void log_info(std::format_string<Args...> fmtstr, Args&&... args)
{
    std::println(stdout, fmtstr, std::forward<Args>(args)...);
}

template <typename... Args>
void log_error(std::format_string<Args...> fmtstr, Args&&... args)
{
    std::println(stderr, fmtstr, std::forward<Args>(args)...);
}

