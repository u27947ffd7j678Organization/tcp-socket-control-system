#include <linux/types.h>

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <bpf/libbpf.h>

#include "tcp_monitor.h"
#include "tcp_monitor.skel.h"

static void print_ascii(const unsigned char *data, __u32 length);
static void print_hex(const unsigned char *data, __u32 length);

static volatile sig_atomic_t exiting = 0;
static __u32 target_pid = 0;

static void handle_signal(int signo)
{
    (void)signo;
    exiting = 1;
}

static int handle_event(void *ctx, void *data, size_t data_size)
{
    const struct event *event = data;

    (void)ctx;

    if (data_size < sizeof(*event))
        return 0;

    if (target_pid != 0 && event->pid != target_pid)
        return 0;

    switch (event->type) {
    case EVENT_CONNECT:
        printf("CONNECT pid=%u comm=%s\n",
               event->pid,
               event->comm);
        break;

    case EVENT_RECV:
        printf("RECV pid=%u comm=%s bytes=%lld data_len=%u data=",
               event->pid,
               event->comm,
               (long long)event->bytes,
               event->data_len);

        print_ascii(event->data, event->data_len);

        printf(" hex=");
        print_hex(event->data, event->data_len);

        putchar('\n');
        break;
    }

    return 0;
}

static void print_ascii(const unsigned char *data, __u32 length)
{
    __u32 i;

    putchar('"');

    for (i = 0; i < length; i++) {
        unsigned char c = data[i];

        switch (c) {
        case '\n':
            printf("\\n");
            break;
        case '\r':
            printf("\\r");
            break;
        case '\t':
            printf("\\t");
            break;
        case '\\':
            printf("\\\\");
            break;
        case '"':
            printf("\\\"");
            break;
        default:
            if (c >= 0x20 && c <= 0x7e)
                putchar(c);
            else
                printf("\\x%02X", c);
            break;
        }
    }

    putchar('"');
}

static void print_hex(const unsigned char *data, __u32 length)
{
    __u32 i;

    for (i = 0; i < length; i++) {
        if (i > 0)
            putchar(' ');

        printf("%02X", data[i]);
    }
}

int main(int argc, char **argv)
{
    struct tcp_monitor_bpf *skeleton = NULL;
    struct ring_buffer *ring_buffer = NULL;
    int error = 0;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    if (argc >= 2)
        target_pid = (__u32)strtoul(argv[1], NULL, 10);

    skeleton = tcp_monitor_bpf__open_and_load();
    if (!skeleton) {
        fprintf(stderr, "Failed to open and load BPF skeleton\n");
        return EXIT_FAILURE;
    }

    error = tcp_monitor_bpf__attach(skeleton);
    if (error) {
        fprintf(stderr, "Failed to attach BPF programs: %d\n", error);
        goto cleanup;
    }

    ring_buffer = ring_buffer__new(
        bpf_map__fd(skeleton->maps.events),
        handle_event,
        NULL,
        NULL
    );

    if (!ring_buffer) {
        error = -errno;
        fprintf(stderr, "Failed to create ring buffer: %d\n", error);
        goto cleanup;
    }

    printf("TCP monitor started. Press Ctrl+C to stop.\n");

    while (!exiting) {
        error = ring_buffer__poll(ring_buffer, 100);
        if (error == -EINTR)
            continue;

        if (error < 0) {
            fprintf(stderr, "Ring buffer polling failed: %d\n", error);
            break;
        }
    }

cleanup:
    ring_buffer__free(ring_buffer);
    tcp_monitor_bpf__destroy(skeleton);

    return error < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
