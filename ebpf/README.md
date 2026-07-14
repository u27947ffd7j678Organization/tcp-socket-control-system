# eBPF TCP Monitor

[日本語版](README.ja.md)

This directory contains an experimental eBPF-based TCP connection monitor for Linux.

The monitor is separate from the TCP server and clients. It observes Linux `connect()` system calls through a tracepoint and prints events from user space.

## Current Scope

- Attach to the `sys_enter_connect` tracepoint.
- Capture event type, process ID, and process command name.
- Send events from kernel space to user space with a BPF ring buffer.
- Print received events to the terminal.

The current implementation does not yet capture peer IP address or port number.

## Files

```text
ebpf/
|-- Makefile
|-- README.md
|-- tcp_monitor.c
|-- tcp_monitor.bpf.c
`-- tcp_monitor.h
```

## Prerequisites

Tested target environment:

- Ubuntu 24.04 LTS
- clang
- gcc
- make
- libelf-dev
- zlib1g-dev
- pkg-config
- libbpf-bootstrap checked out at `~/libbpf-bootstrap`
- `~/libbpf-bootstrap/examples/c` built before this monitor

Install common packages:

```bash
sudo apt install clang llvm make gcc libelf-dev zlib1g-dev pkg-config
```

Build libbpf-bootstrap first:

```bash
cd ~/libbpf-bootstrap/examples/c
make
```

The Makefile expects these generated files:

```text
~/libbpf-bootstrap/examples/c/.output/libbpf/libbpf.a
~/libbpf-bootstrap/examples/c/.output/bpftool/bootstrap/bpftool
~/libbpf-bootstrap/vmlinux.h/include/x86/vmlinux.h
```

## Build

```bash
cd ebpf
make
```

Build flow:

```text
tcp_monitor.bpf.c
  -> clang -target bpf
  -> .output/tcp_monitor.bpf.o
  -> bpftool gen skeleton
  -> .output/tcp_monitor.skel.h
  -> gcc + libbpf + libelf + zlib
  -> tcp_monitor
```

## Run

Loading eBPF programs usually requires elevated privileges:

```bash
sudo ./tcp_monitor
```

Example output:

```text
TCP monitor started. Press Ctrl+C to stop.
type=1 pid=32100 comm=sudo
type=1 pid=496 comm=systemd-resolve
type=1 pid=754 comm=NetworkManager
```

Stop with `Ctrl+C`.

## Clean

```bash
make clean
```

## Limitations

- Captures all `connect()` calls on the host.
- Does not yet filter port `5000`.
- Does not yet display peer IP address or port.
- Does not yet monitor `accept`, `send`, `recv`, or `close`.
- Intended as a monitoring tool; it does not modify the TCP server, clients, or protocol.
