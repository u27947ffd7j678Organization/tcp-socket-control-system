# System Overview

## Project Purpose

This project builds a simple TCP/IP communication control system in C on Ubuntu 24.04.

The goal is to learn socket programming in a practical, visible way: first by documenting the design, then by implementing a TCP server, and later by extending the system with a Windows GUI client, logging, tests, and possible CAN-related integration.

## Background and Learning Goals

The project is designed as a GitHub portfolio project that shows both implementation skill and design process.

Learning goals:

- Understand the basic lifecycle of a TCP server.
- Define a small text-based communication protocol before writing code.
- Keep implementation phases small and reviewable.
- Prepare documents that future Codex tasks can read without guessing intent.
- Build toward an industrial or embedded-style communication example.

## Target Use Case

The system represents a small control and monitoring setup.

A Windows PC client sends command strings over TCP/IP to an Ubuntu TCP server. The server maintains a simple internal state, responds to commands, and will later output logs or connect to simulated sensor/CAN data.

## System Architecture

```text
[Windows Client]
  - Sends commands
  - Receives responses
  - Future GUI display

        TCP/IP

[Ubuntu TCP Server]
  - Waits for client connection
  - Receives command strings
  - Parses commands
  - Updates internal state
  - Sends responses
  - Outputs logs
```

Future expansion:

```text
[Windows GUI Client]
        |
        | TCP/IP
        v
[Ubuntu TCP Server]
        |
        | Future extension
        v
[CAN / STM32 / Sensor Simulator]
```

## Component Roles

### Windows Client

- Connects to the Ubuntu TCP server.
- Sends one command per line.
- Displays server responses.
- Will later become a Windows GUI client.

### Ubuntu TCP Server

- Opens a TCP listen socket.
- Accepts client connections.
- Receives line-based command strings.
- Parses supported commands.
- Updates the internal state when needed.
- Sends one response per command.
- Will later save logs and support configuration files.

### Future CAN / Sensor Simulator

- Provides simulated sensor values or CAN-like data.
- Allows the project to grow toward an embedded communication example.
- Is outside the Phase 2 and Phase 3 implementation scope.

## Current Scope

Phase 2.5 prepares the repository for its first public GitHub release.

Included:

- Finalized repository structure.
- English documents under `docs/en/`.
- Japanese documents under `docs/ja/`.
- Ubuntu setup script.
- README for GitHub visitors.
- Initial Git history and public repository.

Not included:

- C socket implementation.
- Client implementation.
- GUI implementation.
- Logging implementation.
- Unit tests.
- GitHub Actions.

## Future Expansion Plan

- TCP server implementation.
- TCP client implementation.
- Windows GUI client.
- Sensor data simulation.
- CSV logging.
- Configuration file support.
- Multiple client support.
- CAN integration.
- Unit tests.
- GitHub Actions.
- Public portfolio screenshots and release notes.

