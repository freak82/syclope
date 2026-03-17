#include "logging.hpp"
#include "throw.hpp"

static constexpr const char* BPF_PROGRAM = "blah";

int main()
{
    try {
        log_info("Starting with BCC {}", LIBBCC_VERSION);

        ebpf::BPF bpf;

        if (const auto res = bpf.init(BPF_PROGRAM); !res.ok()) {
            throw_runtime_error("bpf.init failed: {}", res.msg());
        }

        // const std::string fnname = bpf.get_syscall_fnname("clone");
        /*
        const std::string fnname = "tcp_sendmsg";
        log_info("Got syscall name: {}", fnname);

        if (const auto res = bpf.attach_kprobe(fnname, "tcp_sendmsg_clone");
            !res.ok()) {
            throw_runtime_error("bpf.attach_kprobe failed: {}", res.msg());
        }
        stdex::scope_exit _([&] {
            if (const auto res = bpf.detach_kprobe(fnname); !res.ok()) {
                log_error("bpf.detach_kprobe failed: {}", res.msg());
            }
        });
        */

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
