#pragma once

namespace curs
{

inline void print(std::string_view str)
{
    ::waddnstr(stdscr, str.data(), static_cast<int>(str.size()));
}

inline void print(int y, int x, std::string_view str)
{
    ::wmove(stdscr, y, x);
    ::waddnstr(stdscr, str.data(), static_cast<int>(str.size()));
}

template <typename... Args>
inline void print(fmt::format_string<Args...> fmtstr, Args&&... args)
{
    fmt::memory_buffer mb;
    fmt::format_to(std::back_inserter(mb), fmtstr, std::forward<Args>(args)...);
    ::waddnstr(stdscr, mb.data(), static_cast<int>(mb.size()));
}

template <typename... Args>
inline void
print(int y, int x, fmt::format_string<Args...> fmtstr, Args&&... args)
{
    fmt::memory_buffer mb;
    fmt::format_to(std::back_inserter(mb), fmtstr, std::forward<Args>(args)...);
    ::wmove(stdscr, y, x);
    ::waddnstr(stdscr, mb.data(), static_cast<int>(mb.size()));
}

} // namespace curs
