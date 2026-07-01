# TCP Socket Control System

[日本語版 README](README.md)

A C language learning project for building a small TCP/IP communication control system on Ubuntu 24.04.

The project is currently in **Phase 2.5: Repository Preparation**. TCP socket communication is intentionally not implemented yet.

## Project Overview

This repository models a simple industrial or embedded-style communication system:

```text
Windows PC Client
        |
        | TCP/IP
        v
Ubuntu Linux TCP Server
```

The first implementation target is an Ubuntu TCP server that receives line-based commands, updates a small internal state, and returns text responses. Later phases will add a Windows GUI client, logging, configuration files, tests, GitHub Actions, and possible CAN or sensor simulation.

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
|-- include/
|-- src/
|-- tests/
|-- scripts/
|   `-- setup_ubuntu.sh
|-- CMakeLists.txt
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

No executable is implemented yet. Use the following commands to verify the CMake project configuration:

```bash
cmake -S . -B build
cmake --build build
```

## Documentation

- English implementation specification: [docs/en/](docs/en/)
- Japanese public documentation: [docs/ja/](docs/ja/)

Main documents:

- [System overview](docs/en/system_overview.md)
- [Protocol specification](docs/en/protocol_spec.md)
- [Communication sequences](docs/en/sequence.md)
- [State transition design](docs/en/state_transition.md)
- [Development log](docs/en/development_log.md)

## Roadmap

- [x] Phase 1: Project skeleton
- [x] Phase 2: TCP/IP system design documents
- [x] Phase 2.5: Repository preparation
- [ ] Phase 3: TCP server implementation
- [ ] Phase 4: TCP client
- [ ] Phase 5: Windows GUI
- [ ] Phase 6: Logging and configuration
- [ ] Phase 7: GitHub Actions and unit tests
- [ ] Phase 8: Portfolio release

## Current Status

The repository is ready for public release as a design-first C networking project. Phase 3 will implement the first TCP server according to `docs/en/protocol_spec.md`.

