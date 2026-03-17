CLANG   = clang
BPFTOOL ?= $(shell command -v bpftool 2>/dev/null || echo /usr/sbin/bpftool)

# Discover BPF programs
BPF_SRCS := $(wildcard *.c)

# Derived files
BPF_OBJS  := $(BPF_SRCS:.c=.o)
BPF_SKELS := $(BPF_SRCS:.c=.skel.h)

# Common BPF compile flags
BPF_CFLAGS := -target bpf -O2 -g -Wall -Wextra -Werror

all: $(BPF_OBJS) $(BPF_SKELS)

# Compile BPF program
%.o: %.c
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

# Generate libbpf skeleton
%.skel.h: %.o
	$(BPFTOOL) gen skeleton $< > $@

clean:
	rm -f $(BPF_OBJS) $(BPF_SKELS)

.PHONY: all clean $(CLANG) $(BPFTOOL)
