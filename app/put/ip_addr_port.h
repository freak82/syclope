#pragma once

#include "put/ip_addr.h"

// TODO: Unit tests
namespace put
{

struct ip4_addr_port
{
    static constexpr auto cnt_addr_bytes = ip4_addr::cnt_bytes;

    ip4_addr addr_;
    __be16 port_;

    [[nodiscard]] static constexpr ip4_addr_port
    from_network_order(__be32 addr, __be16 port) noexcept
    {
        return {
            .addr_ = ip4_addr::from_network_order(addr),
            .port_ = port,
        };
    }
    [[nodiscard]] static constexpr ip4_addr_port
    from_network_order(std::span<const unsigned char, 4> addr,
                       __be16 port) noexcept
    {
        return {
            .addr_ = ip4_addr::from_network_order(addr),
            .port_ = port,
        };
    }
    [[nodiscard]] static constexpr ip4_addr_port
    from_network_order(in_addr addr, __be16 port) noexcept
    {
        return {
            .addr_ = ip4_addr::from_network_order(addr),
            .port_ = port,
        };
    }
    [[nodiscard]] static constexpr ip4_addr_port
    from_network_order(const sockaddr_in& rhs) noexcept
    {
        return from_network_order(rhs.sin_addr, rhs.sin_port);
    }
    [[nodiscard]] static constexpr ip4_addr_port
    from_native_order(uint32_t addr, uint16_t port) noexcept
    {
        return {
            .addr_ = ip4_addr::from_native_order(addr),
            .port_ = ben::native_to_big(port),
        };
    }

    [[nodiscard]] constexpr sockaddr_in to_sockaddr_in() const noexcept
    {
        return {
            .sin_family = AF_INET,
            .sin_port   = port_,
            .sin_addr   = addr_.to_in_addr(),
            .sin_zero   = {},
        };
    }

    constexpr auto operator<=>(const ip4_addr_port&) const noexcept = default;
    constexpr bool operator==(ip4_addr_port rhs) const noexcept
    {
        return (addr_ == rhs.addr_) && (port_ == rhs.port_);
    }
};

struct ip6_addr_port
{
    static constexpr auto cnt_addr_bytes = ip6_addr::cnt_bytes;

    ip6_addr addr_;
    __be16 port_;

    [[nodiscard]] static constexpr ip6_addr_port
    from_network_order(ip6_addr::integer_type addr, __be16 port) noexcept
    {
        return {
            .addr_ = ip6_addr::from_network_order(addr),
            .port_ = port,
        };
    }
    [[nodiscard]] static constexpr ip6_addr_port
    from_network_order(std::span<const unsigned char, 16> addr,
                       __be16 port) noexcept
    {
        return {
            .addr_ = ip6_addr::from_network_order(addr),
            .port_ = port,
        };
    }
    [[nodiscard]] static constexpr ip6_addr_port
    from_network_order(in6_addr addr, __be16 port) noexcept
    {
        return {
            .addr_ = ip6_addr::from_network_order(addr),
            .port_ = port,
        };
    }
    [[nodiscard]] static constexpr ip6_addr_port
    from_network_order(const sockaddr_in6& rhs) noexcept
    {
        return from_network_order(rhs.sin6_addr, rhs.sin6_port);
    }

    [[nodiscard]] constexpr sockaddr_in6 to_sockaddr_in() const noexcept
    {
        return {
            .sin6_family   = AF_INET6,
            .sin6_port     = port_,
            .sin6_flowinfo = {},
            .sin6_addr     = addr_.to_in_addr(),
            .sin6_scope_id = {},
        };
    }

    constexpr auto operator<=>(const ip6_addr_port&) const noexcept = default;
    constexpr bool operator==(const ip6_addr_port& rhs) const noexcept
    {
        return (addr_ == rhs.addr_) && (port_ == rhs.port_);
    }
};

} // namespace put
////////////////////////////////////////////////////////////////////////////////
// The formatters are done in this way so that the parsing functionality of the
// base formatter can be reused and the output to be able to be aligned, etc.
template <>
struct fmt::formatter<put::ip4_addr_port> : fmt::formatter<std::string_view>
{
    auto format(const auto& arg, auto& ctx) const noexcept
    {
        using base_type = fmt::formatter<std::string_view>;
        char buf[INET_ADDRSTRLEN + 8];
        auto [out, _] = fmt::format_to_n(buf, sizeof(buf), "{}:{}", arg.addr_,
                                         ben::big_to_native(arg.port_));
        return base_type::format(
            std::string_view(buf, static_cast<size_t>(out - buf)), ctx);
    }
};

template <>
struct fmt::formatter<put::ip6_addr_port> : fmt::formatter<std::string_view>
{
    auto format(const auto& arg, auto& ctx) const noexcept
    {
        using base_type = fmt::formatter<std::string_view>;
        char buf[INET6_ADDRSTRLEN + 8];
        auto [out, _] = fmt::format_to_n(buf, sizeof(buf), "{}:{}", arg.addr_,
                                         ben::big_to_native(arg.port_));
        return base_type::format(
            std::string_view(buf, static_cast<size_t>(out - buf)), ctx);
    }
};
