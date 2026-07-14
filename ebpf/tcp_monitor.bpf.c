#include "vmlinux.h"

#include <bpf/bpf_helpers.h>

#include "tcp_monitor.h"

char LICENSE[] SEC("license") = "Dual BSD/GPL";

/*
 * Ring buffer used to pass kernel-side events to the user-space monitor.
 * The user-space program reads this map through skeleton->maps.events.
 */
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} events SEC(".maps");

/*
 * Trace connect() calls. This first monitor stage reports PID and command
 * name only; peer IP address and port are planned future extensions.
 */
SEC("tp/syscalls/sys_enter_connect")
int handle_connect(struct trace_event_raw_sys_enter *ctx)
{
    struct event *event;
    __u64 pid_tgid;

    (void)ctx;

    event = bpf_ringbuf_reserve(&events, sizeof(*event), 0);
    if (!event)
        return 0;

    pid_tgid = bpf_get_current_pid_tgid();

    event->type = EVENT_CONNECT;
    event->pid = pid_tgid >> 32;
    bpf_get_current_comm(event->comm, sizeof(event->comm));

    bpf_ringbuf_submit(event, 0);

    return 0;
}
