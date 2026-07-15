#ifndef TCP_MONITOR_H
#define TCP_MONITOR_H

#define TASK_COMM_LEN 16
#define MAX_DATA_LEN 64

enum event_type {
    EVENT_CONNECT = 1,
    EVENT_RECV = 2,
};

struct event {
    __u32 type;
    __u32 pid;
    char comm[TASK_COMM_LEN];

    __s64 bytes;
    __u32 data_len;
    unsigned char data[MAX_DATA_LEN];
};

#endif