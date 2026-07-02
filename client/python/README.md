# Python CLI TCPクライアント

このディレクトリには、Windows側からUbuntu上のC言語TCPサーバへ接続するためのPython製CLIクライアントを配置しています。

## 要件

- Python 3.10以上
- 標準ライブラリのみ使用
- GUIなし

## 実行

リポジトリルートから実行します。

```powershell
python client/python/tcp_client.py --host 192.168.11.54 --port 5000
```

引数:

- `--host`: 接続先サーバのホスト名またはIPアドレス
- `--port`: 接続先TCPポート

デフォルト:

- `--host 127.0.0.1`
- `--port 5000`

## 対話モード

接続後、以下のコマンドを入力できます。

```text
PING
GET_STATUS
START
STOP
RESET
QUIT
```

`help` または `?` でコマンド一覧を表示します。`Ctrl+C` で安全に終了できます。

## 接続失敗時

サーバが起動していない、IPアドレスが違う、ポートが開いていない場合は、分かりやすいエラーメッセージを表示します。

