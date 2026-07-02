# Implementation Notes

## Phase 6: GUI Status Monitor

Phase 6 improves the PySide6 GUI client by parsing `STATUS` responses and showing values in dedicated fields.

### Purpose

- Keep raw communication logs.
- Extract `STATE`, `TEMP`, and `HUMI` from `GET_STATUS` responses.
- Display server state, temperature, humidity, and last update time.
- Keep the server protocol unchanged.

### Parser

The helper function is:

```text
parse_status_response(response: str) -> dict[str, str] | None
```

Accepted response example:

```text
STATUS STATE=RUN TEMP=25.4 HUMI=52.1
```

Parsed result:

```text
STATE=RUN
TEMP=25.4
HUMI=52.1
```

Invalid or incomplete `STATUS` responses return `None`.

### GUI Update Flow

```text
TcpClientWorker receives response
  |
  v
received signal
  |
  v
MainWindow._handle_received()
  |
  +-- append raw receive log
  |
  +-- if response starts with STATUS
        |
        v
      parse_status_response()
        |
        v
      MainWindow._update_status_monitor()
```

Only valid `STATUS` responses update the Status Monitor. Other responses such as `PONG`, `OK START`, `OK STOP`, `OK RESET`, `OK BYE`, and `ERROR UNKNOWN_COMMAND` remain in the communication log only.

### Status Monitor Fields

- State
- Temperature
- Humidity
- Last Update

No automatic polling is added in this phase. The existing `GET_STATUS` button is the manual refresh operation.

## Phase 5: PySide6 GUI TCP Client

Phase 5 adds a GUI TCP client using PySide6.

The GUI client is located at:

```text
client/python_gui/tcp_gui_client.py
```

### Purpose

- Connect to the Ubuntu C TCP server from a desktop GUI.
- Let the user edit host and port values.
- Send protocol commands with buttons.
- Show send and receive logs on screen.
- Show the current connection state.

### Requirements

- Python 3.10 or later.
- PySide6.
- The existing TCP server protocol is not changed.

Install dependencies:

```bash
python -m pip install -r client/python_gui/requirements.txt
```

Run:

```bash
python client/python_gui/tcp_gui_client.py
```

### GUI Controls

- Host input.
- Port input.
- `Connect` button.
- `Disconnect` button.
- Command buttons: `PING`, `GET_STATUS`, `START`, `STOP`, `RESET`, `QUIT`.
- Communication log view.
- Connection status label.

### Threading Model

Socket communication is handled by `TcpClientWorker`, which is moved to a `QThread`.

The GUI thread only handles user interaction and screen updates. Connect, disconnect, and command requests are sent to the worker through Qt signals, and responses are sent back to the GUI through signals.

This keeps the GUI responsive while the client waits for network operations.

### Line-Based Receive

The GUI client receives server responses one line at a time. It keeps an internal byte buffer and returns one decoded line when `\n` is received.

### Phase Boundary

The GUI client does not add new protocol commands and does not modify the C TCP server.

## Phase 4: Python TCP Client

Phase 4 adds a Windows-side CLI client written in Python.

The client is located at:

```text
client/python/tcp_client.py
```

### Purpose

- Connect to the Ubuntu C TCP server from Windows.
- Send protocol commands interactively.
- Display server responses on standard output.
- Keep the implementation simple before building a GUI.

### Requirements

- Python 3.10 or later.
- Standard library only.
- Uses the `socket` module.
- Does not change the TCP server protocol.

### Command-Line Options

```bash
python client/python/tcp_client.py --host 192.168.11.54 --port 5000
```

Options:

- `--host`: server host or IP address.
- `--port`: server TCP port.

Defaults:

- `--host 127.0.0.1`
- `--port 5000`

### Interactive Flow

```text
start client
  |
  v
connect to server
  |
  v
read command from user
  |
  v
send command + "\n"
  |
  v
receive one response line
  |
  v
print response
  |
  v
repeat until QUIT or Ctrl+C
```

Supported command examples:

```text
PING
GET_STATUS
START
STOP
RESET
QUIT
```

### Error Handling

The client prints clear messages for common failures:

- Host name resolution failure.
- Connection refused.
- Connection timeout.
- Other network errors.
- Server-side connection close.

`Ctrl+C` is handled safely and closes the client without a traceback.

### Phase Boundary

No GUI is implemented in Phase 4. The Python client is a CLI tool for communication checks and learning.

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
| `server/src/main.c` | Initializes application state and starts the server. |
| `server/src/server.c` | Owns socket creation, bind, listen, accept, receive, send, and close. |
| `server/src/protocol.c` | Trims line endings, parses commands, updates state, and creates responses. |
| `server/src/app_state.c` | Stores current state and dummy sensor values. |
| `server/include/*.h` | Public declarations shared between source files. |

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
./build/server/tcp_socket_server
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
