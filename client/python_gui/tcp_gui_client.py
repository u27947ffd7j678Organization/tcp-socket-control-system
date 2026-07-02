#!/usr/bin/env python3
"""PySide6 GUI TCP client for the C TCP server."""

from __future__ import annotations

import socket
import sys
from dataclasses import dataclass

from PySide6.QtCore import QObject, QThread, Qt, Signal, Slot
from PySide6.QtWidgets import (
    QApplication,
    QGridLayout,
    QGroupBox,
    QHBoxLayout,
    QLabel,
    QLineEdit,
    QMainWindow,
    QMessageBox,
    QPushButton,
    QPlainTextEdit,
    QSpinBox,
    QVBoxLayout,
    QWidget,
)


DEFAULT_HOST = "192.168.11.54"
DEFAULT_PORT = 5000
RECV_BUFFER_SIZE = 4096
COMMANDS = ("PING", "GET_STATUS", "START", "STOP", "RESET", "QUIT")


@dataclass(frozen=True)
class ConnectionTarget:
    host: str
    port: int


class TcpClientWorker(QObject):
    connected = Signal()
    disconnected = Signal()
    received = Signal(str)
    sent = Signal(str)
    error = Signal(str)

    def __init__(self) -> None:
        super().__init__()
        self._sock: socket.socket | None = None
        self._buffer = bytearray()

    @Slot(str, int)
    def connect_to_server(self, host: str, port: int) -> None:
        if self._sock is not None:
            self.error.emit("Already connected.")
            return

        try:
            sock = socket.create_connection((host, port), timeout=5)
            sock.settimeout(5)
        except socket.gaierror as exc:
            self.error.emit(f"Could not resolve host '{host}': {exc}")
            return
        except ConnectionRefusedError:
            self.error.emit(
                f"Connection refused by {host}:{port}. Check that the server is running."
            )
            return
        except TimeoutError:
            self.error.emit(f"Connection to {host}:{port} timed out.")
            return
        except OSError as exc:
            self.error.emit(f"Network error while connecting: {exc}")
            return

        self._sock = sock
        self._buffer.clear()
        self.connected.emit()

    @Slot()
    def disconnect_from_server(self) -> None:
        self._close_socket()
        self.disconnected.emit()

    @Slot(str)
    def send_command(self, command: str) -> None:
        if self._sock is None:
            self.error.emit("Not connected.")
            return

        try:
            self._sock.sendall((command + "\n").encode("utf-8"))
            self.sent.emit(command)
            response = self._receive_line()
        except TimeoutError:
            self.error.emit("Timed out while waiting for server response.")
            return
        except OSError as exc:
            self.error.emit(f"Network error: {exc}")
            self._close_socket()
            self.disconnected.emit()
            return
        except ConnectionError as exc:
            self.error.emit(str(exc))
            self._close_socket()
            self.disconnected.emit()
            return

        self.received.emit(response)

        if command == "QUIT":
            self._close_socket()
            self.disconnected.emit()

    def _receive_line(self) -> str:
        if self._sock is None:
            raise ConnectionError("Not connected.")

        while b"\n" not in self._buffer:
            chunk = self._sock.recv(RECV_BUFFER_SIZE)
            if not chunk:
                raise ConnectionError("Server closed the connection.")
            self._buffer.extend(chunk)

        line, _, remaining = self._buffer.partition(b"\n")
        self._buffer = bytearray(remaining)
        return line.decode("utf-8", errors="replace").rstrip("\r")

    def _close_socket(self) -> None:
        if self._sock is None:
            return

        try:
            self._sock.close()
        finally:
            self._sock = None
            self._buffer.clear()


class MainWindow(QMainWindow):
    connect_requested = Signal(str, int)
    disconnect_requested = Signal()
    command_requested = Signal(str)

    def __init__(self) -> None:
        super().__init__()
        self.setWindowTitle("TCP Socket GUI Client")
        self.resize(760, 560)

        self._worker_thread = QThread(self)
        self._worker = TcpClientWorker()
        self._worker.moveToThread(self._worker_thread)
        self._worker_thread.start()

        self._build_ui()
        self._connect_signals()
        self._set_connected(False)

    def closeEvent(self, event) -> None:  # type: ignore[override]
        self.disconnect_requested.emit()
        self._worker_thread.quit()
        self._worker_thread.wait(2000)
        super().closeEvent(event)

    def _build_ui(self) -> None:
        central = QWidget(self)
        root_layout = QVBoxLayout(central)

        connection_group = QGroupBox("Connection", central)
        connection_layout = QGridLayout(connection_group)

        self.host_edit = QLineEdit(DEFAULT_HOST, connection_group)
        self.port_spin = QSpinBox(connection_group)
        self.port_spin.setRange(1, 65535)
        self.port_spin.setValue(DEFAULT_PORT)

        self.connect_button = QPushButton("Connect", connection_group)
        self.disconnect_button = QPushButton("Disconnect", connection_group)
        self.status_label = QLabel("Disconnected", connection_group)
        self.status_label.setAlignment(Qt.AlignmentFlag.AlignCenter)

        connection_layout.addWidget(QLabel("Host", connection_group), 0, 0)
        connection_layout.addWidget(self.host_edit, 0, 1)
        connection_layout.addWidget(QLabel("Port", connection_group), 0, 2)
        connection_layout.addWidget(self.port_spin, 0, 3)
        connection_layout.addWidget(self.connect_button, 0, 4)
        connection_layout.addWidget(self.disconnect_button, 0, 5)
        connection_layout.addWidget(QLabel("Status", connection_group), 1, 0)
        connection_layout.addWidget(self.status_label, 1, 1, 1, 5)

        command_group = QGroupBox("Commands", central)
        command_layout = QHBoxLayout(command_group)
        self.command_buttons: dict[str, QPushButton] = {}
        for command in COMMANDS:
            button = QPushButton(command, command_group)
            self.command_buttons[command] = button
            command_layout.addWidget(button)

        log_group = QGroupBox("Communication Log", central)
        log_layout = QVBoxLayout(log_group)
        self.log_view = QPlainTextEdit(log_group)
        self.log_view.setReadOnly(True)
        log_layout.addWidget(self.log_view)

        root_layout.addWidget(connection_group)
        root_layout.addWidget(command_group)
        root_layout.addWidget(log_group, stretch=1)
        self.setCentralWidget(central)

    def _connect_signals(self) -> None:
        self.connect_button.clicked.connect(self._request_connect)
        self.disconnect_button.clicked.connect(self.disconnect_requested.emit)

        for command, button in self.command_buttons.items():
            button.clicked.connect(lambda _checked=False, cmd=command: self._send_command(cmd))

        self.connect_requested.connect(self._worker.connect_to_server)
        self.disconnect_requested.connect(self._worker.disconnect_from_server)
        self.command_requested.connect(self._worker.send_command)

        self._worker.connected.connect(self._on_connected)
        self._worker.disconnected.connect(self._on_disconnected)
        self._worker.sent.connect(lambda command: self._append_log("SEND", command))
        self._worker.received.connect(lambda response: self._append_log("RECV", response))
        self._worker.error.connect(self._on_error)

    def _request_connect(self) -> None:
        target = ConnectionTarget(
            host=self.host_edit.text().strip(),
            port=self.port_spin.value(),
        )
        if not target.host:
            QMessageBox.warning(self, "Input Error", "Host is required.")
            return

        self._append_log("INFO", f"Connecting to {target.host}:{target.port}")
        self.connect_requested.emit(target.host, target.port)

    def _send_command(self, command: str) -> None:
        self.command_requested.emit(command)

    @Slot()
    def _on_connected(self) -> None:
        self._set_connected(True)
        self._append_log("INFO", "Connected.")

    @Slot()
    def _on_disconnected(self) -> None:
        self._set_connected(False)
        self._append_log("INFO", "Disconnected.")

    @Slot(str)
    def _on_error(self, message: str) -> None:
        self._append_log("ERROR", message)
        QMessageBox.warning(self, "TCP Client Error", message)

    def _set_connected(self, connected: bool) -> None:
        self.host_edit.setEnabled(not connected)
        self.port_spin.setEnabled(not connected)
        self.connect_button.setEnabled(not connected)
        self.disconnect_button.setEnabled(connected)
        self.status_label.setText("Connected" if connected else "Disconnected")
        self.status_label.setStyleSheet(
            "color: #0f766e; font-weight: 700;" if connected else "color: #991b1b;"
        )
        for button in self.command_buttons.values():
            button.setEnabled(connected)

    def _append_log(self, direction: str, message: str) -> None:
        self.log_view.appendPlainText(f"[{direction}] {message}")


def main() -> int:
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    return app.exec()


if __name__ == "__main__":
    raise SystemExit(main())

