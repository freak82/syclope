
add_subdirectory(main)

add_executable(tests
    example.cpp
    put/ip_addr.cpp
    $<TARGET_OBJECTS:tests_main>
)

target_include_directories(tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(tests
    PRIVATE
        Boost::endian
        Boost::static_string
        doctest::doctest
        fmt::fmt
        flags_options
        tests_options
)

target_precompile_headers(tests
    PRIVATE precompiled.hpp
)

add_test(
    NAME syclope_tests
    COMMAND tests --force-colors
)
