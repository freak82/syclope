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
    [[nodiscard]] static constexpr size_t size_sockaddr_in() noexcept
    {
        return sizeof(sockaddr_in6);
    }

    constexpr auto operator<=>(const ip6_addr_port&) const noexcept = default;
    constexpr bool operator==(const ip6_addr_port& rhs) const noexcept
    {
        return (addr_ == rhs.addr_) && (port_ == rhs.port_);
    }
};

} // namespace put
////////////////////////////////////////////////////////////////////////////////
template <>
struct fmt::formatter<put::ip4_addr_port>
{
    constexpr auto parse(auto& ctx) const noexcept { return ctx.begin(); }
    auto format(const auto& arg, auto& ctx) const noexcept
    {
        return fmt::format_to(ctx.out(), "{}:{}", arg.addr_,
                              ben::big_to_native(arg.port_));
    }
};

template <>
struct fmt::formatter<put::ip6_addr_port>
{
    constexpr auto parse(auto& ctx) const noexcept { return ctx.begin(); }
    auto format(const auto& arg, auto& ctx) const noexcept
    {
        return fmt::format_to(ctx.out(), "{}:{}", arg.addr_,
                              ben::big_to_native(arg.port_));
    }
};
