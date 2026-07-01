# Protocol Specification

## Purpose

This document defines the text-based protocol used between the client and the Ubuntu TCP server.

The protocol is intentionally simple so the first server implementation can focus on TCP socket handling, command parsing, and predictable responses.

## Basic Rules

- The client sends one command per line.
- The server replies with one response per command.
- Commands and responses are UTF-8 text.
- The line ending is `\n`.
- The server should ignore a trailing `\r` before `\n` for compatibility with Windows clients.
- Commands are case-sensitive.
- Unknown commands return `ERROR UNKNOWN_COMMAND`.
- Malformed input returns `ERROR INVALID_FORMAT`.
- Unexpected server-side failures return `ERROR INTERNAL_ERROR`.

## Connection Model

- The server listens for TCP connections.
- A connected client sends commands sequentially.
- The server processes one command at a time.
- The `QUIT` command closes the client connection after sending `OK BYE`.
- Multi-client behavior is planned for a future phase and is not required in the first server implementation.

## Command List

| Command | Meaning | Example Response |
|---|---|---|
| `GET_STATUS` | Get current server status. | `STATUS STATE=RUN TEMP=25.4 HUMI=52.1` |
| `START` | Change state to `RUN`. | `OK START` |
| `STOP` | Change state to `STOP`. | `OK STOP` |
| `RESET` | Reset internal status. | `OK RESET` |
| `PING` | Check connection health. | `PONG` |
| `QUIT` | Close the current client connection. | `OK BYE` |

## Response Format

Successful command responses use either a command-specific fixed response or a status response.

Examples:

```text
PONG
OK START
OK STOP
OK RESET
OK BYE
STATUS STATE=RUN TEMP=25.4 HUMI=52.1
```

## Error Responses

```text
ERROR UNKNOWN_COMMAND
ERROR INVALID_FORMAT
ERROR INTERNAL_ERROR
```

## Internal State

Initial internal values:

```text
STATE=STOP
TEMP=25.4
HUMI=52.1
```

State changes:

- `START` changes `STATE` to `RUN`.
- `STOP` changes `STATE` to `STOP`.
- `RESET` changes `STATE` to `STOP`.
- `PING`, `GET_STATUS`, and `QUIT` do not change the internal state.

## Example Session

```text
Client -> Server: PING
Server -> Client: PONG

Client -> Server: START
Server -> Client: OK START

Client -> Server: GET_STATUS
Server -> Client: STATUS STATE=RUN TEMP=25.4 HUMI=52.1

Client -> Server: QUIT
Server -> Client: OK BYE
```

## Phase 3 Implementation Notes

The Phase 3 server should implement this protocol without adding extra commands unless the design documents are updated first.

