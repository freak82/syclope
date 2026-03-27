#ifndef SYCLOPE_PRECOMPILED_HPP
#define SYCLOPE_PRECOMPILED_HPP

////////////////////////////////////////////////////////////////////////////////
// general system headers
#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////
// c++ std headers
#include <algorithm>
#include <exception>
#include <experimental/scope>
#include <fstream>
#include <memory>
#include <ranges>
#include <span>
#include <stdexcept>
#include <string>
#include <string_view>
#include <system_error>
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
#include <boost/static_string/static_string.hpp>

////////////////////////////////////////////////////////////////////////////////
// extern library headers
#include <fmt/compile.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/ranges.h>
#include <fmt/std.h>

////////////////////////////////////////////////////////////////////////////////
// this project headers
#include "aliases.hpp"

#endif // SYCLOPE_PRECOMPILED_HPP
