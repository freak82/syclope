#pragma once

namespace mpl // meta programming library
{

template <typename T>
consteval std::string_view type_name() noexcept
{
    std::string_view ret  = __PRETTY_FUNCTION__;
    std::string_view mark = "T = ";
    const auto mlen       = mark.size();
    const auto pos0       = ret.find(mark);
    if (pos0 == std::string_view::npos) throw -1;
    const auto pos1 = ret.find(';', pos0 + mlen);
    if (pos1 == std::string_view::npos) throw -1;
    ret = ret.substr(pos0 + mlen, pos1 - pos0 - mlen);
    return ret;
}

} // namespace mpl
