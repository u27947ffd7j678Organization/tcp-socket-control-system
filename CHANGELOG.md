# Changelog

All notable changes to this project will be documented in this file.

This project follows a phase-based development approach.

## [Unreleased]

### Planned

- Windows GUI client
- CAN integration
- GitHub Actions
- Unit tests

## [0.3.0] - 2026-07-02

### Added

- Added a Python CLI TCP client for Phase 4.
- Added `--host` and `--port` command-line options.
- Added interactive command input for `PING`, `GET_STATUS`, `START`, `STOP`, `RESET`, and `QUIT`.
- Added clear connection error messages and safe `Ctrl+C` handling.
- Updated English and Japanese documentation with Phase 4 notes.

### Notes

- The client uses only the Python standard library.
- The TCP server protocol was not changed.
- GUI implementation remains future work.

## [0.2.0] - 2026-07-01

### Added

- Implemented a basic single-client TCP server in C.
- Added line-based command handling for `PING`, `GET_STATUS`, `START`, `STOP`, `RESET`, and `QUIT`.
- Added internal server state management.
- Added beginner-friendly implementation notes in English and Japanese.
- Added build, run, and manual testing instructions.
- Added `CONTRIBUTING.md` for development rules.

### Notes

- Multi-client support, GUI client, CSV logging, configuration files, and CAN integration remain future work.

## [0.1.0] - 2026-07-01

### Added

- Initial project structure.
- English and Japanese design documents.
- Ubuntu setup script.
- CMake project skeleton.
- README, LICENSE, and `.gitignore`.
- Public GitHub repository.
