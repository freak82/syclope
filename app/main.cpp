#include "logging.hpp"

#include "curs/curses.hpp"
#include "curs/helpers.hpp"

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
void print_info(int y, int x, const auto& key, const auto& val)
{
    using ap_type = AddrPort;
    auto addr     = [](std::span<const unsigned char> bytes) {
        return bytes.first<ap_type::cnt_addr_bytes>();
    };
    const auto src = ap_type::from_network_order(addr(key.saddr), key.sport);
    const auto dst = ap_type::from_network_order(addr(key.daddr), key.dport);
    curs::print(y, x, "src:{} dst:{} sent:{} recv:{}", src, dst, val.sent,
                val.recv);
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
    int y     = 0;
    int x     = 0;
    for (const int fd = ::bpf_map__fd(skel->maps.syclope_conn_map);;) {
        uint32_t count = batch_size;
        const int res =
            ::bpf_map_lookup_batch(fd, &in, &out, keys, vals, &count, nullptr);
        // Print'em all
        for (auto idx : boost::irange(count)) {
            if (keys[idx].is_v4) {
                print_info<put::ip4_addr_port>(y++, x, keys[idx], vals[idx]);
            } else {
                print_info<put::ip6_addr_port>(y++, x, keys[idx], vals[idx]);
            }
        }
        if (res < 0) {
            if (errno == ENOENT) break;
            put::throw_system_error(errno, "Failed to do bpf_map_lookup_batch");
        }
        in = out;
    }
    curs::refresh();
}

int main()
{
    /*
    libbpf_set_print(
        [](enum libbpf_print_level, const char* format, va_list args) {
            return vfprintf(stderr, format, args);
        });
    */
    curs::initscr();
    stdex::scope_exit _(curs::endwin);

    curs::noecho();
    curs::cbreak();
    curs::curs_set(0);

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

        while (running) {
            print_stats(skel);
            ::sleep(1);
        }

        log_info("Done");
    } catch (const std::exception& ex) {
        log_error(ex.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
