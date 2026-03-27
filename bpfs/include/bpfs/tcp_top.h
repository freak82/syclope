#ifndef SYCLOPE_TCP_TOP_H
#define SYCLOPE_TCP_TOP_H

// The information here is in network byte order.
// The current implementation assumes that there will be no connections with
// IPv4 address and the same byte representation of IPv6 address at the same
// time. This allows us to move the `is_v4` flag to the state information.
// The latter allows us to not have "hidden" padding bytes in the key which
// need to be initialized before passing a key to the map functionality.
struct syclope_conn_key
{
    unsigned short sport;
    unsigned short dport;

    unsigned char saddr[16];
    unsigned char daddr[16];
};
// The `conn_key` and `conn_state` are allocated together by the
// BPF_MAP_TYPE_HASH functionality. The `conn_key` already have an alignment of
// 2 and thus there is no point to pack the `conn_state` more.
// Bitfields can't be used because the different fields of single entry can
// potentially be modified concurrently from different functions and using
// bitfields would require locking in that case.
struct __attribute__((packed, aligned(2))) syclope_conn_state
{
    unsigned long long recv; // bytes received from the remote point
    unsigned long long sent; // bytes sent to the remote point
    unsigned char state; // The state of the TCP connection
    bool is_v4;
};

#endif // SYCLOPE_TCP_TOP_H
