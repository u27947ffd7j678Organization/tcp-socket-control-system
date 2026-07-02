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
- Structured status monitor for server state, temperature, humidity, and last update time.
- Socket communication runs in a worker thread so the GUI does not freeze.
- Server responses are received one line at a time.

## Status Monitor

Click `GET_STATUS` to refresh the structured status fields.

The GUI parses responses like:

```text
STATUS STATE=RUN TEMP=25.4 HUMI=52.1
```

Displayed fields:

- State
- Temperature
- Humidity
- Last Update

Other responses such as `PONG`, `OK START`, and `ERROR UNKNOWN_COMMAND` remain in the communication log but do not update the status monitor.

## Screenshots

Connect check:

![PySide6 GUI connection check](../../docs/images/pyside6-gui-connect.png)

All command check:

![PySide6 GUI all command check](../../docs/images/pyside6-gui-command-check.png)

## Notes

- The existing server protocol is not changed.
- This GUI client is separate from the CLI client in `client/python/`.
