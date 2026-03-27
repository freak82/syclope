#pragma once

namespace tcp
{

enum struct state : uint8_t
{
    established    = BPF_TCP_ESTABLISHED,
    syn_sent       = BPF_TCP_SYN_SENT,
    syn_recv       = BPF_TCP_SYN_RECV,
    fin_wait1      = BPF_TCP_FIN_WAIT1,
    fin_wait2      = BPF_TCP_FIN_WAIT2,
    time_wait      = BPF_TCP_TIME_WAIT,
    close          = BPF_TCP_CLOSE,
    close_wait     = BPF_TCP_CLOSE_WAIT,
    last_ack       = BPF_TCP_LAST_ACK,
    listen         = BPF_TCP_LISTEN,
    closing        = BPF_TCP_CLOSING,
    new_syn_recv   = BPF_TCP_NEW_SYN_RECV,
    bound_inactive = BPF_TCP_BOUND_INACTIVE,
};
BOOST_DESCRIBE_ENUM(state,
                    established,
                    syn_sent,
                    syn_recv,
                    fin_wait1,
                    fin_wait2,
                    time_wait,
                    close,
                    close_wait,
                    last_ack,
                    listen,
                    closing,
                    new_syn_recv,
                    bound_inactive);

} // namespace tcp
