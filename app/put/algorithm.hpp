#pragma once

namespace put // project utilities
{

template <stdrg::input_range R>
constexpr bool all_of(R&& rng, const stdrg::range_value_t<R>& val) noexcept
{
    return stdrg::all_of(rng, [&val](const auto& v) { return val == v; });
}

} // namespace put
