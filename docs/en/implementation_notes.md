# Implementation Notes

## Overview

Phase 3 adds the first working TCP server implementation.

The server is intentionally small:

- It listens on TCP port `5000`.
- It accepts one client connection.
- It receives line-based UTF-8 commands.
- It returns one text response per command.
- It closes the connection when the client sends `QUIT`.

Multi-client support, threading, logging files, configuration files, authentication, TLS, and GUI clients are outside this phase.

## Source File Roles

| File | Role |
|---|---|
| `src/main.c` | Initializes application state and starts the server. |
| `src/server.c` | Owns socket creation, bind, listen, accept, receive, send, and close. |
| `src/protocol.c` | Trims line endings, parses commands, updates state, and creates responses. |
| `src/app_state.c` | Stores current state and dummy sensor values. |
| `include/*.h` | Public declarations shared between source files. |

## Processing Flow

```text
main()
  |
  v
initialize AppState
  |
  v
server_run()
  |
  v
socket -> setsockopt -> bind -> listen -> accept
  |
  v
recv command
  |
  v
protocol_handle_command()
  |
  v
send response
  |
  v
repeat until QUIT or disconnect
```

## Socket API Roles

- `socket()` creates an IPv4 TCP socket.
- `setsockopt()` enables `SO_REUSEADDR` so the server can restart quickly.
- `bind()` assigns the socket to `INADDR_ANY` and port `5000`.
- `listen()` starts waiting for incoming client connections.
- `accept()` accepts one connected client.
- `recv()` reads command text from the client.
- `send()` writes response text back to the client.
- `close()` releases client and server sockets.

## Command Parsing

`protocol_handle_command()` receives a mutable command buffer.

It removes trailing line endings:

- `\n`
- `\r\n`

Then it compares the command text with supported command strings.

Supported commands:

- `PING`
- `GET_STATUS`
- `START`
- `STOP`
- `RESET`
- `QUIT`

Unknown commands return:

```text
ERROR UNKNOWN_COMMAND
```

## Internal State

Initial values:

```text
STATE=STOP
TEMP=25.4
HUMI=52.1
```

State changes:

- `START` changes `STATE` to `RUN`.
- `STOP` changes `STATE` to `STOP`.
- `RESET` changes `STATE` to `STOP`.
- `PING`, `GET_STATUS`, and `QUIT` do not change state.

## Build and Run

```bash
cmake -S . -B build
cmake --build build
./build/tcp_socket_server
```

## Test with Netcat

Open another terminal:

```bash
nc 127.0.0.1 5000
```

Commands:

```text
PING
GET_STATUS
START
GET_STATUS
STOP
GET_STATUS
HELLO
QUIT
```

Expected responses:

```text
PONG
STATUS STATE=STOP TEMP=25.4 HUMI=52.1
OK START
STATUS STATE=RUN TEMP=25.4 HUMI=52.1
OK STOP
STATUS STATE=STOP TEMP=25.4 HUMI=52.1
ERROR UNKNOWN_COMMAND
OK BYE
```
