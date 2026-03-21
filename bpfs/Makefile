CLANG   = clang
BPFTOOL ?= $(shell command -v bpftool 2>/dev/null || echo /usr/sbin/bpftool)

# Discover BPF programs
BPF_SRCS := $(wildcard *.c)
SKELS_DIR := ./include/bpfs

# Derived files
BPF_OBJS  := $(BPF_SRCS:.c=.o)
BPF_SKELS := $(addprefix $(SKELS_DIR)/,$(BPF_SRCS:.c=.skel.h))

# Common BPF compile flags
BPF_CFLAGS := -target bpf -D__TARGET_ARCH_x86 \
			  -O2 -g -Wall -Wextra -Werror \
			  -I./include

#.INTERMEDIATE: $(BPF_OBJS)

all: $(BPF_OBJS) $(BPF_SKELS)

# Compile BPF program
%.o: %.c
	$(CLANG) $(BPF_CFLAGS) -c $< -o $@

# Generate libbpf skeleton
$(SKELS_DIR)/%.skel.h: %.o
	$(BPFTOOL) gen skeleton $< > $@

clean:
	rm $(BPF_OBJS) $(BPF_SKELS)

.PHONY: all clean $(CLANG) $(BPFTOOL)
