#include "logging.hpp"

#include "put/skel.hpp"
#include "put/throw.hpp"

#include <bpfs/on_tcp_sendmsg.skel.h>

int main()
{
    try {
        log_info("Starting with LIBBPF {}.{}", LIBBPF_MAJOR_VERSION,
                 LIBBPF_MINOR_VERSION);

        put::skel<on_tcp_sendmsg> skel;

        std::ifstream pipe("/sys/kernel/tracing/trace_pipe");
        for (std::string line;;) {
            if (std::getline(pipe, line)) {
                log_info("Got line: {}", line);
                break;
            } else {
                log_info("Waiting for clone event");
                sleep(1);
            }
        }
    } catch (const std::exception& ex) {
        log_error(ex.what());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
