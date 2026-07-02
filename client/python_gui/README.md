# PySide6 GUI TCP Client

This directory contains a GUI TCP client for the Ubuntu C TCP server.

## Requirements

- Python 3.10 or later
- PySide6

Install dependencies:

```powershell
python -m pip install -r client/python_gui/requirements.txt
```

## Run

From the repository root:

```powershell
python client/python_gui/tcp_gui_client.py
```

## Features

- Host and port inputs.
- Connect and Disconnect buttons.
- Command buttons for `PING`, `GET_STATUS`, `START`, `STOP`, `RESET`, and `QUIT`.
- Send and receive logs.
- Connection status display.
- Socket communication runs in a worker thread so the GUI does not freeze.
- Server responses are received one line at a time.

## Notes

- The existing server protocol is not changed.
- This GUI client is separate from the CLI client in `client/python/`.

