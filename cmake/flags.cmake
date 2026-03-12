
add_library(flags_options INTERFACE)

# Pick some better and more performant architecture
target_compile_options(flags_options
    INTERFACE
        -std=c++2c
        -fno-rtti
        -march=native
        -Wall 
        -Wextra 
        -Wpedantic 
        -Werror
        -Winit-self
        #        -Wuseless-cast
        -Wshadow
        -Wconversion
        -Wsign-conversion
        #        -Warith-conversion
)

# The libbcc links to these dynamically and thus there is no point for us to
# link statically to them.
#target_link_options(flags_options
#    INTERFACE
#        -static-libstdc++ -static-libgcc
#)
