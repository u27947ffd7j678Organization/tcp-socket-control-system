# TCP Socket Control System

[English README](README.en.md)

Ubuntu上のC言語TCPサーバと、Windows側から接続確認できるPythonクライアントを含む、TCP/IP通信学習用プロジェクトです。

現在は **Phase 6: GUIステータスモニタ表示** まで完了しています。

## システム概要

```text
Windows PC
  client/python/tcp_client.py
  client/python_gui/tcp_gui_client.py
        |
        | TCP/IP
        v
Ubuntu Linux
  build/server/tcp_socket_server
```

サーバは行単位のテキストコマンドを受信し、内部状態を更新して応答を返します。クライアントはWindows側から対話モードでコマンドを送信します。

## リポジトリ構成

```text
tcp-socket-control-system/
|-- server/
|   |-- include/
|   |-- src/
|   |-- tests/
|   |-- scripts/
|   |-- CMakeLists.txt
|   `-- README.md
|-- client/
|   |-- python/
|   |   |-- tcp_client.py
|   |   `-- README.md
|   `-- python_gui/
|       |-- tcp_gui_client.py
|       |-- requirements.txt
|       `-- README.md
|-- docs/
|   |-- en/
|   |-- ja/
|   `-- images/
|-- CMakeLists.txt
|-- CHANGELOG.md
|-- CONTRIBUTING.md
|-- README.md
|-- README.en.md
|-- LICENSE
`-- .gitignore
```

## コンポーネント

- [C言語TCPサーバ](server/README.md)
- [Python CLI TCPクライアント](client/python/README.md)
- [PySide6 GUI TCPクライアント](client/python_gui/README.md)

## 通信プロトコル

対応コマンド:

```text
PING
GET_STATUS
START
STOP
RESET
QUIT
```

詳細は [docs/ja/protocol_spec.md](docs/ja/protocol_spec.md) と [docs/en/protocol_spec.md](docs/en/protocol_spec.md) を参照してください。

## 動作確認イメージ

![TCPサーバ応答確認](docs/images/tcp-server-response-check.png)

## Phase 5 GUI動作確認

Connect時:

![PySide6 GUI接続確認](docs/images/pyside6-gui-connect.png)

全コマンド確認:

![PySide6 GUI全コマンド確認](docs/images/pyside6-gui-command-check.png)

## 処理フロー

### サーバ側フローチャート

C言語TCPサーバの起動、接続待ち、コマンド処理、応答送信、切断までの流れです。

```mermaid
flowchart TD
    S1["main()"] --> S2["app_state_init()"]
    S2 --> S3["server_run()"]
    S3 --> S4["create_listen_socket()"]
    S4 --> S5["socket() / bind() / listen()"]
    S5 --> S6["accept()"]
    S6 --> S7["recv()"]
    S7 --> S8["1行コマンドに分割"]
    S8 --> S9["protocol_handle_command()"]

    S9 --> S10{"Command?"}
    S10 -->|PING| S11["PONG"]
    S10 -->|GET_STATUS| S12["STATUS STATE=..."]
    S10 -->|START / STOP / RESET| S13["AppState更新 + OK"]
    S10 -->|QUIT| S14["OK BYE"]
    S10 -->|Unknown| S15["ERROR UNKNOWN_COMMAND"]

    S11 --> S16["send_all()"]
    S12 --> S16
    S13 --> S16
    S14 --> S16
    S15 --> S16

    S16 --> S17{"QUIT or disconnected?"}
    S17 -->|No| S7
    S17 -->|Yes| S18["close(client_fd) / close(server_fd)"]
    S18 --> S19["Server stopped"]
```

### PySide6クライアント側フローチャート

GUI操作は `MainWindow`、通信処理は `TcpClientWorker` が担当します。socket通信は `QThread` 側で動かし、GUIが固まらないようにしています。

```mermaid
flowchart TD
    C1["main()"] --> C2["QApplication / MainWindow"]
    C2 --> C3["MainWindow._build_ui()"]
    C3 --> C4["MainWindow._connect_signals()"]
    C4 --> C5["TcpClientWorker を QThread へ移動"]

    C5 --> C6["Host / Port 入力"]
    C6 --> C7["Connect clicked"]
    C7 --> C8["MainWindow._request_connect()"]
    C8 --> C9["connect_requested signal"]
    C9 --> C10["TcpClientWorker.connect_to_server()"]
    C10 --> C11["socket.create_connection()"]
    C11 --> C12["connected signal"]
    C12 --> C13["MainWindow._on_connected()"]
    C13 --> C14["画面状態を Connected に更新"]

    C14 --> C15["Command button clicked"]
    C15 --> C16["MainWindow._send_command()"]
    C16 --> C17["command_requested signal"]
    C17 --> C18["TcpClientWorker.send_command()"]
    C18 --> C19["socket.sendall(command + newline)"]
    C19 --> C20["TcpClientWorker._receive_line()"]
    C20 --> C21["received signal"]
    C21 --> C22["MainWindow._handle_received()"]
    C22 --> C23["MainWindow._append_log()"]
    C23 --> C24{"STATUS response?"}
    C24 -->|Yes| C25["parse_status_response()"]
    C25 --> C26["MainWindow._update_status_monitor()"]
    C24 -->|No| C27{"QUIT?"}
    C26 --> C27
    C27 -->|No| C15
    C27 -->|Yes| C28["TcpClientWorker._close_socket()"]
    C28 --> C29["disconnected signal"]
    C29 --> C30["画面状態を Disconnected に更新"]
```

### 通信シーケンス図

PySide6 GUI、TCP通信ワーカー、Cサーバ、コマンド処理部の関係です。`SET_LED` は現行プロトコル未対応の例として表現しています。

```mermaid
sequenceDiagram
    participant GUI as PySide6 GUI
    participant Client as TCP Client Worker
    participant Server as TCP Server
    participant Handler as Command Handler

    GUI->>Client: Connect clicked
    Client->>Server: TCP CONNECT
    Server-->>Client: accept()
    Client-->>GUI: connected signal
    GUI->>GUI: status = Connected

    GUI->>Client: PING button
    Client->>Server: "PING\n"
    Server->>Handler: protocol_handle_command("PING")
    Handler-->>Server: "PONG\n"
    Server-->>Client: "PONG\n"
    Client-->>GUI: received("PONG")
    GUI->>GUI: append receive log

    GUI->>Client: GET_STATUS button
    Client->>Server: "GET_STATUS\n"
    Server->>Handler: protocol_handle_command("GET_STATUS")
    Handler-->>Server: "STATUS STATE=STOP TEMP=25.4 HUMI=52.1\n"
    Server-->>Client: status response
    Client-->>GUI: received(status response)
    GUI->>GUI: _handle_received()
    GUI->>GUI: parse_status_response()
    GUI->>GUI: _update_status_monitor()

    GUI->>Client: SET_LED example
    Client->>Server: "SET_LED\n"
    Server->>Handler: protocol_handle_command("SET_LED")
    Handler-->>Server: "ERROR UNKNOWN_COMMAND\n"
    Server-->>Client: error response
    Client-->>GUI: received("ERROR UNKNOWN_COMMAND")
    GUI->>GUI: append receive log

    GUI->>Client: QUIT button
    Client->>Server: "QUIT\n"
    Server->>Handler: protocol_handle_command("QUIT")
    Handler-->>Server: "OK BYE\n"
    Server-->>Client: "OK BYE\n"
    Server->>Server: close sockets
    Client-->>GUI: disconnected signal
    GUI->>GUI: status = Disconnected
```

## ロードマップ

- [x] Phase 1: プロジェクトの土台
- [x] Phase 2: TCP/IPシステム設計書
- [x] Phase 2.5: リポジトリ公開準備
- [x] Phase 3: C言語TCPサーバ
- [x] Phase 4: Python CLI TCPクライアント
- [x] Phase 5: PySide6 GUI TCPクライアント
- [x] Phase 6: GUIステータスモニタ表示
- [ ] Phase 7: GitHub Actions・単体テスト
- [ ] Phase 8: ポートフォリオ公開整備

## ドキュメント

- 英語版の実装仕様: [docs/en/](docs/en/)
- 日本語版の公開説明: [docs/ja/](docs/ja/)
- 変更履歴: [CHANGELOG.md](CHANGELOG.md)
- 開発参加ルール: [CONTRIBUTING.md](CONTRIBUTING.md)
