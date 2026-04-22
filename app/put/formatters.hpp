#pragma once

template <typename Enum>
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
        return base_type::format(
            std::string_view(buf, static_cast<size_t>(out - buf)), ctx);
    }
};
