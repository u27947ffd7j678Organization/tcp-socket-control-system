# 実装メモ

## Phase 6: GUIステータスモニタ

Phase 6 では、PySide6 GUIクライアントで `STATUS` 応答を解析し、専用の表示欄に値を表示するように改善しました。

### 目的

- 生の通信ログは残す。
- `GET_STATUS` の応答から `STATE`、`TEMP`、`HUMI` を取り出す。
- サーバ状態、温度、湿度、最終更新時刻を表示する。
- サーバ側プロトコルは変更しない。

### パーサ

追加したヘルパー関数:

```text
parse_status_response(response: str) -> dict[str, str] | None
```

対応する応答例:

```text
STATUS STATE=RUN TEMP=25.4 HUMI=52.1
```

解析結果:

```text
STATE=RUN
TEMP=25.4
HUMI=52.1
```

不正または不足のある `STATUS` 応答では `None` を返します。

### GUI更新フロー

```text
TcpClientWorker が応答を受信
  |
  v
received signal
  |
  v
MainWindow._handle_received()
  |
  +-- 生の受信ログを表示
  |
  +-- STATUS で始まる応答なら解析
        |
        v
      parse_status_response()
        |
        v
      MainWindow._update_status_monitor()
```

Status Monitorを更新するのは、正常に解析できた `STATUS` 応答だけです。`PONG`、`OK START`、`OK STOP`、`OK RESET`、`OK BYE`、`ERROR UNKNOWN_COMMAND` などは通信ログに残すだけで、専用表示欄は更新しません。

### Status Monitorの表示項目

- State
- Temperature
- Humidity
- Last Update

このフェーズでは自動ポーリングは追加していません。既存の `GET_STATUS` ボタンを手動更新操作として使います。

## Phase 5: PySide6 GUI TCP クライアント

Phase 5 では、PySide6 を使った GUI TCP クライアントを追加しました。

配置場所:

```text
client/python_gui/tcp_gui_client.py
```

### 目的

- デスクトップGUIから Ubuntu 上の C 言語 TCP サーバへ接続する。
- host と port を画面から入力できるようにする。
- ボタン操作でプロトコルコマンドを送信する。
- 送信ログと受信ログを画面に表示する。
- 現在の接続状態を表示する。

### 要件

- Python 3.10以上。
- PySide6。
- 既存のTCPサーバプロトコルは変更しない。

依存関係のインストール:

```bash
python -m pip install -r client/python_gui/requirements.txt
```

実行:

```bash
python client/python_gui/tcp_gui_client.py
```

### GUI操作

- Host入力。
- Port入力。
- `Connect` ボタン。
- `Disconnect` ボタン。
- `PING`、`GET_STATUS`、`START`、`STOP`、`RESET`、`QUIT` ボタン。
- 通信ログ表示。
- 接続状態表示。

### スレッド構成

socket通信は `TcpClientWorker` が担当し、このワーカーを `QThread` 上で動かします。

GUIスレッドは画面操作と表示更新だけを担当します。接続、切断、コマンド送信の要求はQtのシグナルでワーカーへ渡し、サーバ応答もシグナルでGUIへ戻します。

これにより、サーバ応答待ちの間もGUIが固まりにくい構成にしています。

### 1行単位の受信

GUIクライアントはサーバ応答を1行単位で受信します。内部にバイトバッファを持ち、`\n` を受信した時点で1行をデコードして表示します。

### Phase 5 の範囲

GUIクライアントは新しいプロトコルコマンドを追加せず、C言語TCPサーバ側も変更しません。

## Phase 4: Python TCP クライアント

Phase 4 では、Windows側から使える Python 製 CLI クライアントを追加しました。

配置場所:

```text
client/python/tcp_client.py
```

### 目的

- Windows側から Ubuntu 上の C 言語 TCP サーバへ接続する。
- 対話モードでプロトコルコマンドを送信する。
- サーバ応答を標準出力に表示する。
- GUIを作る前に、通信確認用のシンプルなクライアントを用意する。

### 要件

- Python 3.10以上。
- 標準ライブラリのみ使用。
- `socket` モジュールを使用。
- TCPサーバ側のプロトコルは変更しない。

### コマンドライン引数

```bash
python client/python/tcp_client.py --host 192.168.11.54 --port 5000
```

引数:

- `--host`: 接続先サーバのホスト名またはIPアドレス。
- `--port`: 接続先TCPポート。

デフォルト値:

- `--host 127.0.0.1`
- `--port 5000`

### 対話モードの流れ

```text
クライアント起動
  |
  v
サーバへ接続
  |
  v
ユーザー入力を読む
  |
  v
コマンド + "\n" を送信
  |
  v
1行の応答を受信
  |
  v
応答を表示
  |
  v
QUIT または Ctrl+C まで繰り返し
```

入力できるコマンド例:

```text
PING
GET_STATUS
START
STOP
RESET
QUIT
```

### エラー処理

クライアントは、よくある接続失敗を分かりやすく表示します。

- ホスト名解決失敗。
- 接続拒否。
- 接続タイムアウト。
- その他のネットワークエラー。
- サーバ側からの接続切断。

`Ctrl+C` ではトレースバックを出さず、安全に終了します。

### Phase 4 の範囲

Phase 4 ではGUIは実装しません。Pythonクライアントは通信確認と学習用のCLIツールです。

## 概要

Phase 3 では、最初に動作する TCP サーバを実装しました。

今回のサーバは意図的に小さくしています。

- TCP ポート `5000` で待ち受ける。
- 1 つのクライアント接続を受け付ける。
- 行単位の UTF-8 コマンドを受信する。
- 1 コマンドに対して 1 つのテキスト応答を返す。
- クライアントが `QUIT` を送信したら接続を閉じる。

複数クライアント、スレッド、ファイルログ、設定ファイル、認証、TLS、GUI クライアントは今後のフェーズで扱います。

## ソースファイルの役割

| ファイル | 役割 |
|---|---|
| `server/src/main.c` | アプリケーション状態を初期化し、サーバを開始する。 |
| `server/src/server.c` | ソケット作成、bind、listen、accept、受信、送信、close を担当する。 |
| `server/src/protocol.c` | 改行除去、コマンド解析、状態更新、応答生成を担当する。 |
| `server/src/app_state.c` | 現在状態とダミーセンサ値を保持する。 |
| `server/include/*.h` | ソースファイル間で共有する宣言を置く。 |

## 全体の処理流れ

```text
main()
  |
  v
AppState を初期化
  |
  v
server_run()
  |
  v
socket -> setsockopt -> bind -> listen -> accept
  |
  v
コマンド受信
  |
  v
protocol_handle_command()
  |
  v
応答送信
  |
  v
QUIT または切断まで繰り返し
```

## Socket API の役割

- `socket()` は IPv4 TCP ソケットを作成する。
- `setsockopt()` は `SO_REUSEADDR` を有効にし、サーバ再起動時に同じポートを使いやすくする。
- `bind()` はソケットを `INADDR_ANY` とポート `5000` に割り当てる。
- `listen()` はクライアント接続待ち状態にする。
- `accept()` は 1 つのクライアント接続を受け付ける。
- `recv()` はクライアントから送られたコマンド文字列を読む。
- `send()` はサーバ応答をクライアントへ返す。
- `close()` はクライアントソケットとサーバソケットを閉じる。

## コマンド解析

`protocol_handle_command()` は、受信したコマンドバッファを受け取ります。

最初に末尾の改行を取り除きます。

- `\n`
- `\r\n`

その後、対応しているコマンド文字列と比較します。

対応コマンド:

- `PING`
- `GET_STATUS`
- `START`
- `STOP`
- `RESET`
- `QUIT`

未知のコマンドには以下を返します。

```text
ERROR UNKNOWN_COMMAND
```

## 内部状態

初期値:

```text
STATE=STOP
TEMP=25.4
HUMI=52.1
```

状態変更:

- `START` は `STATE` を `RUN` に変更する。
- `STOP` は `STATE` を `STOP` に変更する。
- `RESET` は `STATE` を `STOP` に変更する。
- `PING`、`GET_STATUS`、`QUIT` は状態を変更しない。

## ビルドと実行

```bash
cmake -S . -B build
cmake --build build
./build/server/tcp_socket_server
```

## netcat での確認

別ターミナルで以下を実行します。

```bash
nc 127.0.0.1 5000
```

入力コマンド:

```text
PING
GET_STATUS
START
GET_STATUS
STOP
GET_STATUS
HELLO
QUIT
```

期待される応答:

```text
PONG
STATUS STATE=STOP TEMP=25.4 HUMI=52.1
OK START
STATUS STATE=RUN TEMP=25.4 HUMI=52.1
OK STOP
STATUS STATE=STOP TEMP=25.4 HUMI=52.1
ERROR UNKNOWN_COMMAND
OK BYE
```

## 面接で説明しやすいポイント

- TCP サーバの基本処理を `server.c` にまとめ、プロトコル解析を `protocol.c` に分離している。
- 状態管理を `app_state.c` に分けることで、今後テストや機能追加がしやすい。
- Phase 3 では学習しやすさを優先し、複数クライアントやスレッドはまだ入れていない。
- `docs/en/protocol_spec.md` の仕様に合わせて実装しているため、設計から実装への流れを説明しやすい。
