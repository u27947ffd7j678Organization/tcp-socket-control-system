# Contributing

Thank you for your interest in this project.

This repository is primarily a personal learning and portfolio project, but contributions and suggestions are welcome.

## Development Environment

- Ubuntu 24.04 LTS
- C17
- GCC
- CMake
- GDB

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Manual Test

Start the server:

```bash
./build/server/tcp_socket_server
```

Connect from another terminal:

```bash
nc 127.0.0.1 5000
```

Try these commands:

```text
PING
GET_STATUS
START
GET_STATUS
STOP
RESET
QUIT
```

## Commit Message Convention

Use simple prefixes:

- `docs`: Documentation
- `feat`: New feature
- `fix`: Bug fix
- `refactor`: Code cleanup
- `test`: Test code
- `build`: Build system or setup
- `chore`: Miscellaneous maintenance

Example:

```text
feat: implement basic TCP server
```

## Development Policy

- Keep the code simple and readable.
- Keep Phase 3 limited to a single-client TCP server.
- Update both English and Japanese documentation when specifications change.
- Do not commit build artifacts.
- Do not commit private keys or confidential information.
- Keep commits focused on a single purpose.
- Confirm CMake build before submitting changes.

## Roadmap

The project is developed in phases:

1. Project skeleton
2. Design documents
3. TCP server
4. TCP client
5. Windows GUI
6. Logging and configuration
7. CAN integration
8. GitHub Actions and testing
9. Portfolio release
