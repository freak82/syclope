
add_executable(syclope
    main.cpp
)

target_link_libraries(syclope
    PRIVATE 
        bcc
        Boost::program_options
        fmt::fmt
        flags_options
)

target_precompile_headers(syclope
    PRIVATE precompiled.hpp
)

