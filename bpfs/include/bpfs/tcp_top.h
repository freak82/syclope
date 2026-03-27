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

struct syclope_conn_state
{
    unsigned long long recv; // bytes received from the remote point
    unsigned long long sent  : 56; // bytes sent to the remote point
    unsigned long long state : 7; // The state of the TCP connection
    unsigned long long is_v4 : 1;
};

#endif // SYCLOPE_TCP_TOP_H
