# Changelog

All notable changes to this project will be documented in this file.

This project follows a phase-based development approach.

## [Unreleased]

### Changed

- Reorganized the repository into `server/` and `client/` directories.
- Moved the C TCP server implementation under `server/`.
- Moved the Python CLI client to `client/python/`.
- Split detailed usage into `server/README.md` and `client/python/README.md`.
- Simplified the root README as a system overview.

### Planned

- CAN integration
- GitHub Actions
- Unit tests

## [0.4.0] - 2026-07-02

### Added

- Added a PySide6 GUI TCP client under `client/python_gui/`.
- Added host and port inputs.
- Added Connect and Disconnect buttons.
- Added command buttons for `PING`, `GET_STATUS`, `START`, `STOP`, `RESET`, and `QUIT`.
- Added send and receive logs in the GUI.
- Added connection status display.
- Added worker-thread based communication so the GUI remains responsive.
- Added `requirements.txt` and GUI client README.

### Notes

- The existing TCP server protocol was not changed.
- The existing Python CLI client remains available under `client/python/`.

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
