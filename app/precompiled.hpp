#ifndef SYCLOPE_PRECOMPILED_HPP
#define SYCLOPE_PRECOMPILED_HPP

////////////////////////////////////////////////////////////////////////////////
// general system headers
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
// c++ std headers
#include <algorithm>
#include <chrono>
#include <exception>
#include <experimental/scope>
#include <fstream>
#include <memory>
#include <optional>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
#include <type_traits>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// other system headers
#include <bpf/bpf.h>
#include <linux/bpf.h>
#include <ncursesw/ncurses.h>

////////////////////////////////////////////////////////////////////////////////
// boost headers
#include <boost/describe/enum.hpp>
#include <boost/describe/enumerators.hpp>
#include <boost/describe/enum_to_string.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/range/irange.hpp>

////////////////////////////////////////////////////////////////////////////////
// extern library headers
#include <fmt/compile.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

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

#endif // SYCLOPE_PRECOMPILED_HPP
