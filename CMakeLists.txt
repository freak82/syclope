cmake_minimum_required(VERSION 4.2)

project(
    syclope
    DESCRIPTION "System performance observability tool"
    LANGUAGES CXX C ASM
    VERSION 0.0.1
)

option(
    SYCLOPE_BUILD_TESTS
    "Enable building tests and test infrastructure. Default: ON. Values: { ON, OFF }."
    ON
)

include(cmake/flags.cmake)

add_subdirectory(extern)

add_subdirectory(app)

if(SYCLOPE_BUILD_TESTS)
    include(CTest)
    include(cmake/tests.cmake)
    add_subdirectory(tests)
endif()
