cmake_minimum_required(VERSION 4.2)

project(
    learn-ebpf 
    DESCRIPTION "A project to learn ebpf"
    LANGUAGES CXX C ASM
    VERSION 0.0.1
)

include(cmake/flags.cmake)

add_executable(learn-ebpf
    src/main.cpp
)

target_precompile_headers(learn-ebpf
    PRIVATE src/precompiled.hpp
)

target_link_libraries(learn-ebpf
    PRIVATE 
        bcc
        flags_options
)
