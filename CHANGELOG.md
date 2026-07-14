# Changelog

All notable changes to this project will be documented in this file.

This project follows a phase-based development approach.

## [Unreleased]

### Added

- Added an experimental eBPF TCP monitor under `ebpf/`.
- Added eBPF monitor documentation and build/run notes.
- Added `.gitignore` rules for eBPF build outputs.
- Added portfolio-focused README structure for Phase 8.
- Added final demo, system architecture, and software architecture images to the repository preview.
- Moved the English README to `docs/en/README.md`.
- Added GitHub Actions CI for CMake build, CTest, and pytest.
- Added Python pytest coverage for `parse_status_response()`.
- Added `requirements-dev.txt` for test dependencies.
- Added server test planning notes under `server/tests/`.
- Added structured status monitor to the PySide6 GUI client.
- Parsed `STATUS` responses and displayed state, temperature, humidity, and last update time.

### Changed

- Reorganized the repository into `server/` and `client/` directories.
- Moved the C TCP server implementation under `server/`.
- Moved the Python CLI client to `client/python/`.
- Split detailed usage into `server/README.md` and `client/python/README.md`.
- Reorganized the root README as a professional portfolio overview.
- Changed the root README preview to Japanese.
- Replaced architecture images with resized versions.

### Planned

- eBPF peer IP/port display
- eBPF port 5000 filtering
- eBPF send/recv/close monitoring
- CAN integration
- STM32 integration
- CSV logging
- Periodic polling
- Docker support
- Authentication

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
