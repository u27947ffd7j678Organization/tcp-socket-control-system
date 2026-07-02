# Development Log

## Phase 4 - Python TCP Client

### Summary

Added a Python CLI client for communication checks from Windows.

### Added

- `client/python/tcp_client.py`.
- `--host` and `--port` command-line options.
- Interactive command input for the existing TCP server protocol.
- Standard output display for server responses.
- Safe `Ctrl+C` handling.
- Clear connection error messages.
- README and implementation note updates.

### Notes

- The client uses only the Python standard library.
- No GUI was added in this phase.
- The TCP server protocol was not changed.

## Phase 3 - Basic TCP Server

### Summary

Implemented the first working single-client TCP server in C.

### Added

- Basic TCP server listening on port `5000`.
- Line-based command handling for `PING`, `GET_STATUS`, `START`, `STOP`, `RESET`, and `QUIT`.
- Internal state management for `STOP` and `RUN`.
- Standard output logs for startup, connection, receive, send, and shutdown events.
- Build, run, and manual test instructions.
- Implementation notes.
- `CHANGELOG.md` and `CONTRIBUTING.md`.

### Notes

- Multi-client support and GUI clients are planned for future phases.
- CSV logging, configuration files, CAN integration, and tests are not part of Phase 3.

## Phase 2.5 - Repository Preparation

### Summary

Prepared the project for its first public GitHub release.

### Added

- Language-specific documentation directories.
- Ubuntu setup script.
- GitHub-ready README.
- Public release roadmap.
- Initial Git repository structure.

### Notes

- No socket implementation was added in this phase.
- English documents under `docs/en/` are the implementation specification.
- Japanese documents under `docs/ja/` are for public explanation and interview preparation.

## Phase 2 - Design Documents

### Summary

Created the basic design documents for the TCP/IP communication control system.

### Added

- System overview.
- Protocol specification.
- Communication sequence examples.
- State transition design.
- Japanese public documentation.
- README roadmap update.

### Notes

- No socket implementation was added in this phase.
- The protocol is line-based UTF-8 text.
- Phase 3 should implement the server according to `docs/en/protocol_spec.md`.

## Phase 1 - Project Skeleton

### Summary

Created the initial project structure.

### Added

- Minimal CMake configuration for C17.
- README.
- Initial documentation placeholders.
- `.gitignore`.
- MIT license.

### Notes

Socket communication was intentionally deferred.
