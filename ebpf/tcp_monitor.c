#include <linux/types.h>

#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <bpf/libbpf.h>

#include "tcp_monitor.h"
#include "tcp_monitor.skel.h"

static volatile sig_atomic_t exiting = 0;

static void handle_signal(int signo)
{
    (void)signo;
    exiting = 1;
}

static int handle_event(void *ctx, void *data, size_t data_size)
{
    const struct event *event = data;

    (void)ctx;

    if (data_size < sizeof(*event)) {
        fprintf(stderr, "Received incomplete event\n");
        return 0;
    }

    printf("type=%u pid=%u comm=%s\n",
           event->type,
           event->pid,
           event->comm);

    return 0;
}

int main(void)
{
    struct tcp_monitor_bpf *skeleton = NULL;
    struct ring_buffer *ring_buffer = NULL;
    int error = 0;

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

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
