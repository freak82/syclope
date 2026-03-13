
add_library(tests_options INTERFACE)

target_compile_definitions(tests_options
    INTERFACE
        DOCTEST_CONFIG_USE_STD_HEADERS
        DOCTEST_CONFIG_SUPER_FAST_ASSERTS
        DOCTEST_CONFIG_NO_SHORT_MACRO_NAMES
        DOCTEST_CONFIG_NO_COMPARISON_WARNING_SUPPRESSION
)

target_compile_options(tests_options
    INTERFACE
        -fno-omit-frame-pointer
        -fsanitize=address,leak,pointer-compare,pointer-subtract,undefined
        -fsanitize-address-use-after-scope
)

target_link_options(tests_options
    INTERFACE
        -fsanitize=address,leak,pointer-compare,pointer-subtract,undefined
)

