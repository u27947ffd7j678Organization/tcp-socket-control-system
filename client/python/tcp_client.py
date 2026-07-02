#!/usr/bin/env python3
"""Interactive TCP client for the Phase 3 C TCP server."""

from __future__ import annotations

import argparse
import socket
import sys


DEFAULT_HOST = "127.0.0.1"
DEFAULT_PORT = 5000
RECV_BUFFER_SIZE = 4096


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Interactive TCP client for tcp_socket_server."
    )
    parser.add_argument(
        "--host",
        default=DEFAULT_HOST,
        help=f"server host or IP address (default: {DEFAULT_HOST})",
    )
    parser.add_argument(
        "--port",
        type=int,
        default=DEFAULT_PORT,
        help=f"server TCP port (default: {DEFAULT_PORT})",
    )
    return parser.parse_args()


def receive_line(sock: socket.socket) -> str:
    data = bytearray()

    while True:
        chunk = sock.recv(RECV_BUFFER_SIZE)
        if not chunk:
            raise ConnectionError("server closed the connection")

        data.extend(chunk)
        if b"\n" in chunk:
            break

    line, _, _ = data.partition(b"\n")
    return line.decode("utf-8", errors="replace")


def print_help() -> None:
    print("Available commands:")
    print("  PING")
    print("  GET_STATUS")
    print("  START")
    print("  STOP")
    print("  RESET")
    print("  QUIT")
    print("  help")


def run_client(host: str, port: int) -> int:
    try:
        with socket.create_connection((host, port), timeout=5) as sock:
            print(f"[INFO] Connected to {host}:{port}")
            print_help()

            while True:
                try:
                    command = input("tcp> ").strip()
                except EOFError:
                    print()
                    break

                if not command:
                    continue

                if command.lower() in {"help", "?"}:
                    print_help()
                    continue

                sock.sendall((command + "\n").encode("utf-8"))
                response = receive_line(sock)
                print(response)

                if command == "QUIT":
                    break

    except KeyboardInterrupt:
        print("\n[INFO] Interrupted by user. Closing client.")
        return 0
    except socket.gaierror as exc:
        print(f"[ERROR] Could not resolve host '{host}': {exc}", file=sys.stderr)
        return 1
    except ConnectionRefusedError:
        print(
            f"[ERROR] Connection refused by {host}:{port}. "
            "Check that the TCP server is running.",
            file=sys.stderr,
        )
        return 1
    except TimeoutError:
        print(
            f"[ERROR] Connection to {host}:{port} timed out.",
            file=sys.stderr,
        )
        return 1
    except OSError as exc:
        print(f"[ERROR] Network error: {exc}", file=sys.stderr)
        return 1
    except ConnectionError as exc:
        print(f"[ERROR] {exc}", file=sys.stderr)
        return 1

    print("[INFO] Client closed.")
    return 0


def main() -> int:
    args = parse_args()
    return run_client(args.host, args.port)


if __name__ == "__main__":
    raise SystemExit(main())
