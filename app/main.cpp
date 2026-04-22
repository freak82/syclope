#include "logging.hpp"

#include "curs/curses.hpp"
#include "curs/helpers.hpp"

#include "put/ip_addr.hpp"
#include "put/skel.hpp"
#include "put/throw.hpp"

#include "tcp/state.hpp"

#include <bpfs/tcp_top.h>
#include <bpfs/tcp_top.skel.h>

// We need to be sure that we can represent all needed states
static_assert(std::numeric_limits<decltype(syclope_conn_state::state)>::max() >=
              BPF_TCP_MAX_STATES);

static volatile sig_atomic_t running = true;
static void signal_handler(int)
{
    running = false;
}

template <typename Addr>
static void print_info(int y, int x, const auto& key, const auto& val)
{
    [[maybe_unused]] auto addr = [](std::span<const unsigned char> bytes) {
        return Addr::from_network_order(bytes.first<Addr::cnt_bytes>());
    };
    // clang-format off
    curs::print(y, x, 
                 "{:>16}{:>6}{:>16}{:>6}{:>13}{:>13}{:>14}", 
                 addr(key.saddr), ben::big_to_native(key.sport),
                 addr(key.daddr), ben::big_to_native(key.dport),
                 val.sent, val.recv, tcp::state(val.state));
    // clang-format on
}

static uint32_t print_stats(put::skel<tcp_top>& skel)
{
    // The batch size needs to be at least of the size of the biggest bucket
    // in the hash-table or the function returns ENOSPC.
    // https://elixir.bootlin.com/linux/v6.19.6/source/kernel/bpf/hashtab.c#L1764
    static constexpr size_t batch_size = 5;
    syclope_conn_key keys[batch_size];
    syclope_conn_state vals[batch_size];

    struct entry
    {
        syclope_conn_key key;
        syclope_conn_state val;
    };
    std::vector<entry> data;

    void* in  = nullptr;
    void* out = nullptr;
    curs::print(0, 0, "{:>16}{:>6}{:>16}{:>6}{:>13}{:>13}{:>14}", "raddr",
                "rport", "laddr", "lport", "sent bytes", "recv bytes", "state");
    for (const int fd = ::bpf_map__fd(skel->maps.syclope_conn_map);;) {
        uint32_t count = batch_size;
        const int res =
            ::bpf_map_lookup_batch(fd, &in, &out, keys, vals, &count, nullptr);
        for (auto idx : boost::irange(count)) {
            data.push_back({.key = keys[idx], .val = vals[idx]});
        }
        if (res < 0) {
            if (errno == ENOENT) break;
            put::throw_system_error(errno, "Failed to do bpf_map_lookup_batch");
        }
        in = out;
    }

    // Print'em all
    stdrg::sort(data, [](const auto& lhs, const auto& rhs) {
        return ben::big_to_native(lhs.key.dport) <
               ben::big_to_native(rhs.key.dport);
    });
    for (int y = 1; const auto& ent : data) {
        if (ent.val.is_v4) {
            print_info<put::ip4_addr>(y++, 0, ent.key, ent.val);
        } else {
            print_info<put::ip6_addr>(y++, 0, ent.key, ent.val);
        }
    }

    return static_cast<uint32_t>(data.size());
}

int main()
{
    curs::initscr();
    stdex::scope_exit _(curs::endwin);

    curs::noecho();
    curs::cbreak();
    curs::curs_set(0);

    libbpf_set_print(
        [](enum libbpf_print_level lvl, const char* format, va_list args) {
            if (lvl == LIBBPF_WARN) {
                curs::vw_printw(stdscr, format, args);
                curs::wrefresh(stdscr);
            }
            return 0;
            // return vfprintf(stderr, format, args);
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

        while (running) {
            curs::wclear(stdscr);
            [[maybe_unused]] const auto cnt = print_stats(skel);
            curs::wrefresh(stdscr);
            // wnoutrefresh(stdscr);
            // doupdate();
            ::sleep(1);
        }

        log_info("Done");
    } catch (const std::exception& ex) {
        log_error(ex.what());
        ::wgetch(stdscr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
