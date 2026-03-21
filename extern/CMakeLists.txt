include(FetchContent)

set(BOOST_ENABLE_CMAKE ON)
set(BOOST_INCLUDE_LIBRARIES 
    endian
    program_options
    range
    static_string
)

FetchContent_Declare(
  Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.90.0/boost-1.90.0-cmake.7z
  URL_HASH SHA256=218e74c4aa362a994b7b7a23b2920f455a00205c656405fcf262cf60b8871921
  USES_TERMINAL_DOWNLOAD TRUE
  DOWNLOAD_NO_EXTRACT FALSE
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(Boost)

################################################################################

FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        12.1.0
)
FetchContent_MakeAvailable(fmt)

################################################################################

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG v2.4.12
    OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(doctest)

################################################################################

find_package(Curses REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBBPF REQUIRED libbpf)
