#include <vmlinux.h>
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_endian.h>
#include <bpf/bpf_tracing.h>

#define AF_INET 2 /* Internet IP Protocol */

SEC("fentry/tcp_send_msg")
int BPF_PROG(on_tcp_sendmsg, struct sock* sk, struct msghdr* msg, size_t size) {
    (void)ctx;
    (void)msg;
    (void)size;

    const u16 family = BPF_CORE_READ(sk, __sk_common.skc_family);
    if (family != AF_INET) return 0;

    const u16 lport = bpf_ntohs(BPF_CORE_READ(sk, __sk_common.skc_num));
    const u16 dport = bpf_ntohs(BPF_CORE_READ(sk, __sk_common.skc_dport));

    bpf_printk("tcp_send_msg: src=%hu, dst=%hu\n", lport, dport);

    return 0;
}
