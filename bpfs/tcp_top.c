#include <vmlinux.h>
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_endian.h>
#include <bpf/bpf_tracing.h>

#include "bpfs/tcp_top.h"

#define AF_INET  2 /* Internet IP Protocol	*/
#define AF_INET6 10 /* IP version 6			*/

const char LICENSE[] SEC("license") = "GPL";

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 128);
    __type(key, struct syclope_conn_key);
    __type(value, struct syclope_conn_state);
} syclope_conn_map SEC(".maps");

static u16 fill_key(struct sock* sk, struct syclope_conn_key* out) {
    const u16 family = BPF_CORE_READ(sk, __sk_common.skc_family);
    if (family != AF_INET && family != AF_INET6) return 0;
    // The local port (skc_num) is in host byte order
    // It's converted because to be consistent with the rest of the information:
    // everything reported is in network byte order.
    out->sport = BPF_CORE_READ(sk, __sk_common.skc_dport),
    out->dport = bpf_htons(BPF_CORE_READ(sk, __sk_common.skc_num));
    if (family == AF_INET) {
        // The (s/d)addr field is bigger than the IPv4 address size and thus
        // we need to zero out the remaining bytes.
        __builtin_memset(out->saddr, 0, sizeof(out->saddr));
        __builtin_memset(out->daddr, 0, sizeof(out->daddr));
        bpf_probe_read_kernel(out->daddr, sizeof(sk->__sk_common.skc_rcv_saddr),
                              &sk->__sk_common.skc_rcv_saddr);
        bpf_probe_read_kernel(out->saddr, sizeof(sk->__sk_common.skc_daddr),
                              &sk->__sk_common.skc_daddr);
    } else {
        bpf_probe_read_kernel(out->daddr,
                              sizeof(sk->__sk_common.skc_v6_rcv_saddr),
                              &sk->__sk_common.skc_v6_rcv_saddr);
        bpf_probe_read_kernel(out->saddr, sizeof(sk->__sk_common.skc_v6_daddr),
                              &sk->__sk_common.skc_v6_daddr);
    }
    return family;
}

static void fill_stats(struct sock* sk, size_t nbytes, bool is_recv) {
    struct syclope_conn_key key;
    const u16 family = fill_key(sk, &key);
    if (family == 0) return;

    struct syclope_conn_state* s = bpf_map_lookup_elem(&syclope_conn_map, &key);
    if (!s) {
        const struct syclope_conn_state zero = {
            .recv  = 0,
            .sent  = 0,
            .state = TCP_ESTABLISHED,
            .is_v4 = (family == AF_INET),
        };
        bpf_map_update_elem(&syclope_conn_map, &key, &zero, BPF_NOEXIST);
        s = bpf_map_lookup_elem(&syclope_conn_map, &key);
        if (!s) return; // Is this really needed (possible to happen)?
    }
    // The assumption is that recvmsg/sendmsg will never be called concurrently
    // for the same operation/direction and the same socket.
    // Thus we modify the sent/recv bytes without atomic operations.
    if (is_recv) {
        s->recv += nbytes;
    } else {
        s->sent += nbytes;
    }
}

SEC("fentry/tcp_recvmsg")
int BPF_PROG(syclope_tcp_recvmsg,
             struct sock* sk,
             struct msghdr* msg,
             size_t len) {
    (void)ctx;
    (void)msg;
    fill_stats(sk, len, true);
    return 0;
}

SEC("fentry/tcp_sendmsg")
int BPF_PROG(syclope_tcp_sendmsg,
             struct sock* sk,
             struct msghdr* msg,
             size_t len) {
    (void)ctx;
    (void)msg;
    fill_stats(sk, len, false);
    return 0;
}

SEC("fentry/tcp_set_state")
int BPF_PROG(tcp_set_state, struct sock* sk, int state) {
    (void)ctx;
    if (state == TCP_SYN_SENT || state == TCP_SYN_RECV) return 0;
    // The connection entry is created by the sendmsg/recvmsg hooks because
    // some connections may have been already established when the hooks
    // are loaded and we're going to miss them if we add entries here.
    struct syclope_conn_key key;
    const u16 family = fill_key(sk, &key);
    if (family == 0) return 0;
    struct syclope_conn_state* s = bpf_map_lookup_elem(&syclope_conn_map, &key);
    if (s) s->state = state;
    return 0;
}

