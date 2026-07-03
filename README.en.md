# TCP Socket Control System

[Main README](README.md) / [Japanese docs](docs/ja/)

## Project Overview

TCP Socket Control System is a portfolio project that demonstrates TCP/IP communication between a Linux C server and a Windows Python client.

The system includes:

- A TCP socket server implemented in C on Ubuntu.
- A PySide6 GUI client implemented in Python on Windows.
- A command protocol for `PING`, `GET_STATUS`, `START`, `STOP`, `RESET`, and `QUIT`.
- A GUI status monitor that displays `STATE`, `TEMP`, and `HUMI` values received from the server.
- GitHub Actions CI for CMake build checks, CTest, and Python unit tests.

The implementation is complete through **Phase 8: GitHub Portfolio Refinement**.

## Demo Screenshot

![TCP Socket Control System demo](docs/images/pyside6-gui-status-monitor-phase6.png)

The Windows GUI client communicates with the Linux TCP server over TCP/IP.

Implemented features:

- TCP connection and disconnection.
- `PING` / `PONG` communication.
- `GET_STATUS` response parsing.
- `START`, `STOP`, and `RESET` commands.
- `QUIT` command handling.
- Status Monitor.
- Communication Log.

## System Architecture

![System architecture](docs/images/system-architecture.png)

The Windows 11 GUI client connects to the Ubuntu 24.04 LTS TCP server over TCP/IP. The server listens on port `5000` and responds to line-based text commands sent by the client.

## Software Architecture

![Software architecture](docs/images/software-architecture.png)

Main responsibilities:

- `MainWindow`: GUI presentation, user interaction, Status Monitor, and Communication Log.
- `TcpClientWorker`: connection management, command sending, response receiving, and GUI notification.
- `TCP Socket Server`: client acceptance, command receiving, protocol parsing, AppState update, and response sending.
- `AppState`: current server state storage for `STATE`, `TEMP`, and `HUMI`.

## Sequence Diagram

This sequence shows how the PySide6 GUI, TCP worker, C server, and command handler cooperate during connection and command processing. `SET_LED` is shown as an unsupported command example.

```mermaid
sequenceDiagram
    participant GUI as PySide6 GUI
    participant Client as TCP Client Worker
    participant Server as TCP Server
    participant Handler as Command Handler

    GUI->>Client: Connect clicked
    Client->>Server: TCP CONNECT
    Server-->>Client: accept()
    Client-->>GUI: connected signal
    GUI->>GUI: status = Connected

    GUI->>Client: PING button
    Client->>Server: "PING\n"
    Server->>Handler: protocol_handle_command("PING")
    Handler-->>Server: "PONG\n"
    Server-->>Client: "PONG\n"
    Client-->>GUI: received("PONG")
    GUI->>GUI: append receive log

    GUI->>Client: GET_STATUS button
    Client->>Server: "GET_STATUS\n"
    Server->>Handler: protocol_handle_command("GET_STATUS")
    Handler-->>Server: "STATUS STATE=STOP TEMP=25.4 HUMI=52.1\n"
    Server-->>Client: status response
    Client-->>GUI: received(status response)
    GUI->>GUI: _handle_received()
    GUI->>GUI: parse_status_response()
    GUI->>GUI: _update_status_monitor()

    GUI->>Client: SET_LED example
    Client->>Server: "SET_LED\n"
    Server->>Handler: protocol_handle_command("SET_LED")
    Handler-->>Server: "ERROR UNKNOWN_COMMAND\n"
    Server-->>Client: error response
    Client-->>GUI: received("ERROR UNKNOWN_COMMAND")
    GUI->>GUI: append receive log

    GUI->>Client: QUIT button
    Client->>Server: "QUIT\n"
    Server->>Handler: protocol_handle_command("QUIT")
    Handler-->>Server: "OK BYE\n"
    Server-->>Client: "OK BYE\n"
    Server->>Server: close sockets
    Client-->>GUI: disconnected signal
    GUI->>GUI: status = Disconnected
```

## Flowchart

The server flow shows startup, socket setup, client connection, command handling, response sending, and shutdown.

```mermaid
flowchart TD
    S1["main()"] --> S2["app_state_init()"]
    S2 --> S3["server_run()"]
    S3 --> S4["create_listen_socket()"]
    S4 --> S5["socket() / bind() / listen()"]
    S5 --> S6["accept()"]
    S6 --> S7["recv()"]
    S7 --> S8["Split into command lines"]
    S8 --> S9["protocol_handle_command()"]

    S9 --> S10{"Command?"}
    S10 -->|PING| S11["PONG"]
    S10 -->|GET_STATUS| S12["STATUS STATE=..."]
    S10 -->|START / STOP / RESET| S13["Update AppState + OK"]
    S10 -->|QUIT| S14["OK BYE"]
    S10 -->|Unknown| S15["ERROR UNKNOWN_COMMAND"]

    S11 --> S16["send_all()"]
    S12 --> S16
    S13 --> S16
    S14 --> S16
    S15 --> S16

    S16 --> S17{"QUIT or disconnected?"}
    S17 -->|No| S7
    S17 -->|Yes| S18["close(client_fd) / close(server_fd)"]
    S18 --> S19["Server stopped"]
```

The PySide6 client flow keeps socket communication in `TcpClientWorker` on a `QThread`, so the GUI remains responsive while commands are sent and responses are received.

```mermaid
flowchart TD
    C1["main()"] --> C2["QApplication / MainWindow"]
    C2 --> C3["MainWindow._build_ui()"]
    C3 --> C4["MainWindow._connect_signals()"]
    C4 --> C5["Move TcpClientWorker to QThread"]

    C5 --> C6["Enter Host / Port"]
    C6 --> C7["Connect clicked"]
    C7 --> C8["MainWindow._request_connect()"]
    C8 --> C9["connect_requested signal"]
    C9 --> C10["TcpClientWorker.connect_to_server()"]
    C10 --> C11["socket.create_connection()"]
    C11 --> C12["connected signal"]
    C12 --> C13["MainWindow._on_connected()"]
    C13 --> C14["Update UI to Connected"]

    C14 --> C15["Command button clicked"]
    C15 --> C16["MainWindow._send_command()"]
    C16 --> C17["command_requested signal"]
    C17 --> C18["TcpClientWorker.send_command()"]
    C18 --> C19["socket.sendall(command + newline)"]
    C19 --> C20["TcpClientWorker._receive_line()"]
    C20 --> C21["received signal"]
    C21 --> C22["MainWindow._handle_received()"]
    C22 --> C23["MainWindow._append_log()"]
    C23 --> C24{"STATUS response?"}
    C24 -->|Yes| C25["parse_status_response()"]
    C25 --> C26["MainWindow._update_status_monitor()"]
    C24 -->|No| C27{"QUIT?"}
    C26 --> C27
    C27 -->|No| C15
    C27 -->|Yes| C28["TcpClientWorker._close_socket()"]
    C28 --> C29["disconnected signal"]
    C29 --> C30["Update UI to Disconnected"]
```

## Technology Stack

### Server

- C
- POSIX Socket
- CMake
- Linux, tested on Ubuntu 24.04 LTS

### Client

- Python 3.10 or later
- PySide6
- Python standard library `socket`

### Development

- Git
- GitHub
- VS Code
- SSH

### CI

- GitHub Actions
- pytest
- CTest

## Directory Structure

```text
tcp-socket-control-system/
|-- .github/
|   `-- workflows/
|       `-- ci.yml
|-- client/
|   |-- python/
|   |   |-- tcp_client.py
|   |   `-- README.md
|   `-- python_gui/
|       |-- tcp_gui_client.py
|       |-- status_parser.py
|       |-- requirements.txt
|       `-- README.md
|-- docs/
|   |-- en/
|   |-- ja/
|   `-- images/
|-- server/
|   |-- include/
|   |-- scripts/
|   |-- src/
|   |-- tests/
|   |-- CMakeLists.txt
|   `-- README.md
|-- tests/
|   `-- python/
|-- CMakeLists.txt
|-- CHANGELOG.md
|-- CONTRIBUTING.md
|-- README.md
|-- README.en.md
|-- requirements-dev.txt
`-- LICENSE
```

## Build & Run

### Server

Build the C TCP server on Linux:

```bash
cmake -S . -B build
cmake --build build
```

Run the server:

```bash
./build/server/tcp_socket_server
```

The server listens on port `5000` by default.

### Python CLI Client

Run the standard-library CLI client:

```bash
python client/python/tcp_client.py --host 192.168.11.54 --port 5000
```

### PySide6 GUI Client

Create a virtual environment and install GUI dependencies:

```bash
cd client/python_gui
python -m venv .venv
.venv\Scripts\activate
python -m pip install -r requirements.txt
python tcp_gui_client.py
```

On Linux or macOS, use `source .venv/bin/activate` instead of `.venv\Scripts\activate`.

## GitHub Actions

GitHub Actions runs on `push` and `pull_request`.

The CI workflow checks:

- C server configuration and build with CMake.
- CTest entry point.
- Python unit tests with pytest.

Run the same checks locally:

```bash
python -m pip install -r requirements-dev.txt
pytest

cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Documentation

- Server details: [server/README.md](server/README.md)
- CLI client details: [client/python/README.md](client/python/README.md)
- GUI client details: [client/python_gui/README.md](client/python_gui/README.md)
- Protocol specification: [docs/en/protocol_spec.md](docs/en/protocol_spec.md)
- Japanese documentation: [docs/ja/](docs/ja/)
- Changelog: [CHANGELOG.md](CHANGELOG.md)
- Contribution rules: [CONTRIBUTING.md](CONTRIBUTING.md)

## Future Extensions

- SocketCAN integration.
- STM32 integration.
- CSV logging.
- Periodic status polling.
- Docker support.
- Authentication.
