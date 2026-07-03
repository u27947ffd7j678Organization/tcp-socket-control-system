# Server Tests

This directory is reserved for C server tests.

Current CI coverage for the server is intentionally small:

- Configure the project with CMake.
- Build the `tcp_socket_server` executable.
- Run `ctest` as the test entry point.

Future server-side tests may include:

- Protocol command unit tests.
- AppState state transition tests.
- Socket-level integration tests.
- Error-handling tests for invalid input.

The server protocol should remain compatible with `docs/en/protocol_spec.md`.

