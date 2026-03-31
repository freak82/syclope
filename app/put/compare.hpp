#pragma once

namespace put // project utilities
{

template <typename T>
    requires(std::is_trivially_copyable_v<T> &&
             std::has_unique_object_representations_v<T>)
inline int mem_compare(const T& lhs, const T& rhs) noexcept
{
    return ::memcmp(&lhs, &rhs, sizeof(T));
}

template <typename T>
    requires(std::is_trivially_copyable_v<T> &&
             std::has_unique_object_representations_v<T>)
inline int mem_compare(const T& lhs,
                       std::span<const unsigned char, sizeof(T)> rhs) noexcept
{
    return ::memcmp(&lhs, rhs.data(), sizeof(T));
}

template <typename T>
    requires(std::is_trivially_copyable_v<T> &&
             std::has_unique_object_representations_v<T>)
inline int mem_compare(std::span<const unsigned char, sizeof(T)> lhs,
                       const T& rhs) noexcept
{
    return ::memcmp(lhs.data(), &rhs, sizeof(T));
}

} // namespace put
