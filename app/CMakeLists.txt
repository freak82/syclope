
add_executable(syclope
    main.cpp
)

add_dependencies(syclope bpfs)

# The skeletons are added as a system include directory because they
# generate some warnings otherwise.
target_include_directories(syclope SYSTEM
    PRIVATE
        ${CMAKE_SOURCE_DIR}/bpfs/include
)
target_include_directories(syclope
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(syclope
    PRIVATE 
        bpf
        curses
        Boost::endian
        Boost::program_options
        Boost::range
        Boost::static_string
        fmt::fmt
        flags_options
)

target_precompile_headers(syclope
    PRIVATE precompiled.hpp
)

