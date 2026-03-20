#ifndef SYCLOPE_TCP_TOP_H
#define SYCLOPE_TCP_TOP_H

// The information here is in network byte order
struct syclope_conn_key
{
    unsigned short sport;
    unsigned short dport;

    unsigned char saddr[16];
    unsigned char daddr[16];

    bool is_v4;
};

struct syclope_traffic
{
    unsigned long long sent;
    unsigned long long recv;
};

#endif // SYCLOPE_TCP_TOP_H
