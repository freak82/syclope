
add_subdirectory(main)

add_executable(tests
    put/ip_addr.cpp
    put/ip_addr_port.cpp
    $<TARGET_OBJECTS:tests_main>
)

target_include_directories(tests
    PRIVATE
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/app
)

target_link_libraries(tests
    PRIVATE
        Boost::endian
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
