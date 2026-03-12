#pragma once

// clang-format off

template <typename... Args>
[[gnu::cold]]
[[noreturn]] 
inline void throw_runtime_error(std::format_string<Args...> fmtstr,
                                Args&&... args)
{
    std::string mb; 
    mb.reserve(128);// TODO: Optimize with in-place/stack storage
    std::format_to(std::back_inserter(mb), fmtstr, std::forward<Args>(args)...);
    throw std::runtime_error(mb);
}

template <typename... Args>
[[gnu::cold]]
[[noreturn]] 
inline void throw_system_error(int err, 
                               std::format_string<Args...> fmtstr, 
                               Args&&... args)
{
    std::string mb; 
    mb.reserve(128);// TODO: Optimize with in-place/stack storage
    std::format_to(std::back_inserter(mb), fmtstr, std::forward<Args>(args)...);
    throw std::system_error(err, std::system_category(), mb);
}

// clang-format on
