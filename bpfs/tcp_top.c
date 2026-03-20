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
    __type(value, struct syclope_traffic);
} syclope_conn_map SEC(".maps");

static void fill_stats(struct sock* sk, size_t rbytes, size_t sbytes) {
    const u16 family = BPF_CORE_READ(sk, __sk_common.skc_family);
    if (family != AF_INET && family != AF_INET6) return;
    // The local port (skc_num) is in host byte order
    // It's converted because to be consistent with the rest of the information:
    // everything reported is in network byte order.
    struct syclope_conn_key key = {
        .sport = BPF_CORE_READ(sk, __sk_common.skc_dport),
        .dport = bpf_htons(BPF_CORE_READ(sk, __sk_common.skc_num)),
        .saddr = {},
        .daddr = {},
        .is_v4 = (family == AF_INET),
    };
    if (family == AF_INET) {
        bpf_probe_read_kernel(&key.daddr, sizeof(sk->__sk_common.skc_rcv_saddr),
                              &sk->__sk_common.skc_rcv_saddr);
        bpf_probe_read_kernel(&key.saddr, sizeof(sk->__sk_common.skc_daddr),
                              &sk->__sk_common.skc_daddr);
    } else {
        bpf_probe_read_kernel(key.daddr,
                              sizeof(sk->__sk_common.skc_v6_rcv_saddr),
                              &sk->__sk_common.skc_v6_rcv_saddr);
        bpf_probe_read_kernel(key.saddr, sizeof(sk->__sk_common.skc_v6_daddr),
                              &sk->__sk_common.skc_v6_daddr);
    }

    struct syclope_traffic* traf = bpf_map_lookup_elem(&syclope_conn_map, &key);
    if (traf) {
        traf->sent += sbytes;
        traf->recv += rbytes;
        bpf_map_update_elem(&syclope_conn_map, &key, traf, BPF_EXIST);
    } else {
        const struct syclope_traffic tmp = {
            .sent = sbytes,
            .recv = rbytes,
        };
        bpf_map_update_elem(&syclope_conn_map, &key, &tmp, BPF_NOEXIST);
    }
}

SEC("fentry/tcp_recvmsg")
int BPF_PROG(syclope_tcp_recvmsg,
             struct sock* sk,
             struct msghdr* msg,
             size_t len) {
    (void)ctx;
    (void)msg;
    fill_stats(sk, len, 0);
    return 0;
}

SEC("fentry/tcp_sendmsg")
int BPF_PROG(syclope_tcp_sendmsg,
             struct sock* sk,
             struct msghdr* msg,
             size_t len) {
    (void)ctx;
    (void)msg;
    fill_stats(sk, 0, len);
    return 0;
}
