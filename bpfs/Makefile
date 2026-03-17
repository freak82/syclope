CLANG   = clang
BPFTOOL ?= $(shell command -v bpftool 2>/dev/null || echo /usr/sbin/bpftool)

# Discover BPF programs
BPF_SRCS := $(wildcard *.c)
SKELS_DIR := ./skels/bpfs

# Derived files
BPF_OBJS  := $(BPF_SRCS:.c=.o)
BPF_SKELS := $(BPF_SRCS:.c=.skel.h)

# Common BPF compile flags
BPF_CFLAGS := -target bpf -O2 -g -Wall -Wextra -Werror

.INTERMEDIATE: $(BPF_OBJS)

all: skelsdir $(BPF_OBJS) $(BPF_SKELS)

# Compile BPF program
%.o: %.c
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

# Generate libbpf skeleton
%.skel.h: %.o
	$(BPFTOOL) gen skeleton $< > $(SKELS_DIR)/$@

skelsdir:
	mkdir -p $(SKELS_DIR)

clean:
	rm -rf $(SKELS_DIR)

.PHONY: all clean skelsdir $(CLANG) $(BPFTOOL)
