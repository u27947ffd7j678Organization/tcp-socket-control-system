# C言語TCPサーバ

このディレクトリには、Ubuntu上で動作するC言語TCPサーバを配置しています。

## 構成

```text
server/
|-- include/
|   |-- app_state.h
|   |-- protocol.h
|   `-- server.h
|-- src/
|   |-- app_state.c
|   |-- main.c
|   |-- protocol.c
|   `-- server.c
|-- tests/
|-- scripts/
|   `-- setup_ubuntu.sh
|-- CMakeLists.txt
`-- README.md
```

## Ubuntuセットアップ

Ubuntu 24.04上で必要なパッケージをインストールします。

```bash
chmod +x server/scripts/setup_ubuntu.sh
./server/scripts/setup_ubuntu.sh
```

## ビルド

リポジトリルートから実行します。

```bash
cmake -S . -B build
cmake --build build
```

実行ファイル:

```text
build/server/tcp_socket_server
```

## 実行

```bash
./build/server/tcp_socket_server
```

サーバはTCPポート `5000` で待ち受けます。

## netcatでの確認

別ターミナルから接続します。

```bash
nc 127.0.0.1 5000
```

入力例:

```text
PING
GET_STATUS
START
GET_STATUS
STOP
RESET
QUIT
```

## 注意

- Phase 3ではシングルクライアントのみ対応しています。
- 複数クライアント、スレッド、設定ファイル、CSVログは今後のフェーズで扱います。
