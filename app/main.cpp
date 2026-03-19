#include "logging.hpp"

#include "put/ip_addr_port.h"
#include "put/skel.hpp"
#include "put/throw.hpp"

#include <bpfs/tcp_top.h>
#include <bpfs/tcp_top.skel.h>

static volatile sig_atomic_t running = true;
static void signal_handler(int)
{
    running = false;
}

template <typename AddrPort>
void print_info(const auto& key, const auto& val)
{
    using ap_type  = AddrPort;
    const auto src = ap_type::from_network_order(key.saddr, key.sport);
    const auto dst = ap_type::from_network_order(key.saddr, key.sport);
    fmt::println("src:{} dst:{} sent:{} recv:{}", src, dst, val.sent, val.recv);
}

static void print_stats(put::skel<tcp_top>& skel)
{
    // The batch size needs to be at least of the size of the biggest bucket
    // in the hash-table or the function returns ENOSPC.
    // https://elixir.bootlin.com/linux/v6.19.6/source/kernel/bpf/hashtab.c#L1764
    static constexpr size_t batch_size = 5;
    syclope_conn_key keys[batch_size];
    syclope_traffic vals[batch_size];

    void* in  = nullptr;
    void* out = nullptr;
    for (const int fd = ::bpf_map__fd(skel->maps.syclope_conn_map);;) {
        uint32_t count = batch_size;
        const int res =
            ::bpf_map_lookup_batch(fd, &in, &out, keys, vals, &count, nullptr);
        if (res < 0) {
            put::throw_system_error(errno, "Failed to do bpf_map_lookup_batch");
        }
        in = out;
        // Print'em all
        for (auto idx : boost::irange(count)) {
            if (key.is_v4) {
                print_info<put::ip4_addr_port>(keys[idx], vals[idx]);
            } else {
                print_info<put::ip6_addr_port>(keys[idx], vals[idx]);
            }
        }
    }
}

int main()
{
    libbpf_set_print(
        [](enum libbpf_print_level, const char* format, va_list args) {
            return vfprintf(stderr, format, args);
        });

    try {
        log_info("Starting with LIBBPF {}.{}", LIBBPF_MAJOR_VERSION,
                 LIBBPF_MINOR_VERSION);

        if (::signal(SIGINT, signal_handler) == SIG_ERR) {
            put::throw_system_error(errno, "Failed to set signal handler");
        }

        // TODO: In order to set some parameters the skel needs:
        // 1. Opened
        // 2. Set parameters
        // 3. Loaded and attached
        put::skel<tcp_top> skel;

        while (running) { print_stats(skel); }

    } catch (const std::exception& ex) {
        log_error(ex.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
