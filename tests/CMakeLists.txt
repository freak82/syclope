
add_subdirectory(main)

add_executable(tests
    example.cpp
    $<TARGET_OBJECTS:tests_main>
)

#target_include_directories(tests
#    PUBLIC
#        ${CMAKE_SOURCE_DIR}/src
#)

target_link_libraries(tests
    PRIVATE
        doctest::doctest
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

