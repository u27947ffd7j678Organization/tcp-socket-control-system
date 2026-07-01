# State Transition Design

## Purpose

This document defines the server state model used by the protocol.

The first implementation uses only two states to keep the TCP server easy to understand and test.

## Initial State

```text
STOP
```

## States

| State | Meaning |
|---|---|
| `STOP` | The server is stopped or idle. |
| `RUN` | The server is running. |

## Transitions

```text
STOP -- START --> RUN
RUN  -- STOP  --> STOP
RUN  -- RESET --> STOP
STOP -- RESET --> STOP
```

## Transition Table

| Current State | Command | Next State | Response |
|---|---|---|---|
| `STOP` | `START` | `RUN` | `OK START` |
| `RUN` | `START` | `RUN` | `OK START` |
| `RUN` | `STOP` | `STOP` | `OK STOP` |
| `STOP` | `STOP` | `STOP` | `OK STOP` |
| `RUN` | `RESET` | `STOP` | `OK RESET` |
| `STOP` | `RESET` | `STOP` | `OK RESET` |

## No State Change Commands

The following commands do not change the state:

- `PING`
- `GET_STATUS`
- `QUIT`

## Invalid Commands

Unknown or invalid commands do not change the state.

Examples:

```text
HELLO -> ERROR UNKNOWN_COMMAND
empty line -> ERROR INVALID_FORMAT
```

