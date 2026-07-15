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

struct recv_args {
    const void *buf;
};

struct {
    __uint(type, BPF_MAP_TYPE_HASH);
    __uint(max_entries, 1024);
    __type(key, __u64);
    __type(value, struct recv_args);
} recv_args_map SEC(".maps");

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
    event->bytes = 0;
    event->data_len = 0;
    bpf_get_current_comm(event->comm, sizeof(event->comm));

    bpf_ringbuf_submit(event, 0);

    return 0;
}

SEC("tp/syscalls/sys_enter_recvfrom")
int handle_recv_enter(struct trace_event_raw_sys_enter *ctx)
{
    __u64 pid_tgid;
    struct recv_args args = {};

    pid_tgid = bpf_get_current_pid_tgid();

    /*
     * recvfrom(sockfd, buf, len, flags, src_addr, addrlen)
     *
     * args[0] = sockfd
     * args[1] = buf
     * args[2] = len
     */
    args.buf = (const void *)ctx->args[1];

    bpf_map_update_elem(
        &recv_args_map,
        &pid_tgid,
        &args,
        BPF_ANY
    );

    return 0;
}

SEC("tp/syscalls/sys_exit_recvfrom")
int handle_recv_exit(struct trace_event_raw_sys_exit *ctx)
{
    struct recv_args *args;
    struct event *event;
    __u64 pid_tgid;
    __u32 copy_len;
    long ret;

    pid_tgid = bpf_get_current_pid_tgid();
    ret = ctx->ret;

    args = bpf_map_lookup_elem(&recv_args_map, &pid_tgid);
    if (!args)
        return 0;

    /* Delete the saved ENTER arguments after the EXIT side is processed. */
    if (ret <= 0) {
        bpf_map_delete_elem(&recv_args_map, &pid_tgid);
        return 0;
    }

    copy_len = ret;
    if (copy_len > MAX_DATA_LEN)
        copy_len = MAX_DATA_LEN;

    event = bpf_ringbuf_reserve(&events, sizeof(*event), 0);
    if (!event) {
        bpf_map_delete_elem(&recv_args_map, &pid_tgid);
        return 0;
    }

    event->type = EVENT_RECV;
    event->pid = pid_tgid >> 32;
    event->bytes = ret;
    event->data_len = copy_len;

    bpf_get_current_comm(event->comm, sizeof(event->comm));

    /*
     * Copy the bytes written by recvfrom() from the user-space buffer.
     * The copied payload is capped at MAX_DATA_LEN.
     */
    if (bpf_probe_read_user(
            event->data,
            copy_len,
            args->buf) < 0) {
        event->data_len = 0;
    }

    bpf_ringbuf_submit(event, 0);

    bpf_map_delete_elem(&recv_args_map, &pid_tgid);

    return 0;
}
