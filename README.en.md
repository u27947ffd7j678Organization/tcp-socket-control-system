# TCP Socket Control System

[日本語版 README](README.md)

A C language learning project for building a small TCP/IP communication control system on Ubuntu 24.04.

The project is currently in **Phase 4: Python TCP Client**. A CLI client can connect from Windows to the Ubuntu C TCP server.

## Project Overview

This repository models a simple industrial or embedded-style communication system:

```text
Windows PC Client
        |
        | TCP/IP
        v
Ubuntu Linux TCP Server
```

The first implementation is an Ubuntu TCP server that receives line-based commands, updates a small internal state, and returns text responses. Later phases will add a Windows GUI client, logging, configuration files, tests, GitHub Actions, and possible CAN or sensor simulation.

## Learning Objectives

- Learn TCP/IP socket programming in C step by step.
- Define a protocol before implementing the server.
- Keep architecture and sequence decisions visible in Markdown.
- Build a repository that is easy to review, clone, and extend.
- Prepare a public GitHub portfolio project with both English and Japanese documentation.

## Development Environment

- OS: Ubuntu 24.04
- Language: C17
- Build system: CMake
- Compiler: gcc
- Debugger: gdb
- IDE: VS Code with Remote SSH
- Version control: Git

## Repository Structure

```text
tcp-socket-control-system/
|-- docs/
|   |-- en/
|   |-- ja/
|   `-- images/
|-- clients/
|   `-- python/
|       `-- tcp_client.py
|-- include/
|-- src/
|-- tests/
|-- scripts/
|   `-- setup_ubuntu.sh
|-- CHANGELOG.md
|-- CMakeLists.txt
|-- CONTRIBUTING.md
|-- README.md
|-- README.en.md
|-- LICENSE
`-- .gitignore
```

## Ubuntu Setup

Run the setup script on Ubuntu 24.04:

```bash
chmod +x scripts/setup_ubuntu.sh
./scripts/setup_ubuntu.sh
```

The script installs:

- `build-essential`
- `cmake`
- `git`
- `gdb`
- `pkg-config`

## Build Procedure

Build the `tcp_socket_server` executable:

```bash
cmake -S . -B build
cmake --build build
```

Expected executable path:

```text
build/tcp_socket_server
```

## Run Server

```bash
./build/tcp_socket_server
```

The server listens on TCP port `5000`.

## Manual Test

![TCP server response check](docs/images/tcp-server-response-check.png)

### Using netcat on Ubuntu

Connect from another terminal on Ubuntu:

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
RESET
QUIT
```

From Windows PowerShell, if the Ubuntu server IP is `192.168.11.54`:

```powershell
Test-NetConnection 192.168.11.54 -Port 5000
```

For interactive testing, use `telnet` or another TCP client tool.

### Using the Python Client from Windows

The Python client requires Python 3.10 or later and uses only the standard library.

```powershell
python clients/python/tcp_client.py --host 192.168.11.54 --port 5000
```

After connecting, use the interactive prompt:

```text
PING
GET_STATUS
START
STOP
RESET
QUIT
```

Press `Ctrl+C` to close the client safely. No GUI is implemented in this phase.

## Documentation

- English implementation specification: [docs/en/](docs/en/)
- Japanese public documentation: [docs/ja/](docs/ja/)

Main documents:

- [System overview](docs/en/system_overview.md)
- [Protocol specification](docs/en/protocol_spec.md)
- [Communication sequences](docs/en/sequence.md)
- [State transition design](docs/en/state_transition.md)
- [Implementation notes](docs/en/implementation_notes.md)
- [Development log](docs/en/development_log.md)

## Roadmap

- [x] Phase 1: Project skeleton
- [x] Phase 2: TCP/IP system design documents
- [x] Phase 2.5: Repository preparation
- [x] Phase 3: TCP server implementation
- [x] Phase 4: Python TCP client
- [ ] Phase 5: Windows GUI
- [ ] Phase 6: Logging and configuration
- [ ] Phase 7: GitHub Actions and unit tests
- [ ] Phase 8: Portfolio release

## Current Status

This is a design-first networking learning project. The basic TCP server follows `docs/en/protocol_spec.md`, and the Python CLI client can be used from Windows for communication checks.
