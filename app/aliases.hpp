#pragma once

////////////////////////////////////////////////////////////////////////////////
/// std aliases
namespace stdex = std::experimental;
namespace stdrg = std::ranges;
namespace stdvw = std::views;

////////////////////////////////////////////////////////////////////////////////
/// boost libraries aliases
namespace ben   = boost::endian;
namespace bdesc = boost::describe;
namespace bpo   = boost::program_options;
namespace bss   = boost::static_strings;

////////////////////////////////////////////////////////////////////////////////
/// Generic functionality

// The max value of given field of given type/struct. Works for bitfields too.
#define SYCLOPE_MAX_FIELD_VALUE(type, field) \
    [] {                                     \
        type t{};                            \
        return --t.field;                    \
    }()

////////////////////////////////////////////////////////////////////////////////
/// Generic formatters
template <class Enum>
    requires bdesc::has_describe_enumerators<Enum>::value
struct fmt::formatter<Enum> : fmt::formatter<std::string_view>
{
    auto format(const Enum& e, auto& ctx) const noexcept
    {
        using base_type = fmt::formatter<std::string_view>;
        if (const char* s = bdesc::enum_to_string(e, nullptr); s) {
            return base_type::format(std::string_view(s), ctx);
        }
        char buf[128];
        auto [out, _] = fmt::format_to_n(buf, sizeof(buf), "unknown({})",
                                         std::to_underlying(e));
        return base_type::format(std::string_view(buf, out - buf), ctx);
    }
};
