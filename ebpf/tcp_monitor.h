#ifndef TCP_MONITOR_H
#define TCP_MONITOR_H

#define TASK_COMM_LEN 16

enum event_type {
    EVENT_CONNECT = 1,
};

struct event {
    __u32 type;
    __u32 pid;
    char comm[TASK_COMM_LEN];
};

#endif
