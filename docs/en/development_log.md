# Development Log

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

