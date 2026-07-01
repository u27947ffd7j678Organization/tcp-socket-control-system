# 通信プロトコル仕様

## 目的

この文書は、クライアントと Ubuntu TCP サーバ間で使用するテキストベースの通信プロトコルを定義します。

最初のサーバ実装では、TCP ソケット処理、コマンド解析、予測しやすい応答に集中できるよう、プロトコルは意図的にシンプルにします。

## 基本ルール

- クライアントは 1 行に 1 コマンドを送信する。
- サーバは 1 コマンドに対して 1 応答を返す。
- コマンドと応答は UTF-8 テキストとする。
- 改行コードは `\n` とする。
- Windows クライアントとの互換性のため、サーバは `\n` の直前にある `\r` を無視してよい。
- コマンドは大文字小文字を区別する。
- 未知のコマンドには `ERROR UNKNOWN_COMMAND` を返す。
- 形式不正の入力には `ERROR INVALID_FORMAT` を返す。
- サーバ内部エラーには `ERROR INTERNAL_ERROR` を返す。

## 接続モデル

- サーバは TCP 接続を待ち受ける。
- 接続済みクライアントはコマンドを順番に送信する。
- サーバは 1 度に 1 つのコマンドを処理する。
- `QUIT` コマンドでは、`OK BYE` を送信した後にクライアント接続を閉じる。
- 複数クライアント対応は将来フェーズで扱い、最初のサーバ実装では必須としない。

## コマンド一覧

| コマンド | 意味 | 応答例 |
|---|---|---|
| `GET_STATUS` | 現在のサーバ状態を取得する。 | `STATUS STATE=RUN TEMP=25.4 HUMI=52.1` |
| `START` | 状態を `RUN` に変更する。 | `OK START` |
| `STOP` | 状態を `STOP` に変更する。 | `OK STOP` |
| `RESET` | 内部状態をリセットする。 | `OK RESET` |
| `PING` | 接続確認を行う。 | `PONG` |
| `QUIT` | 現在のクライアント接続を閉じる。 | `OK BYE` |

## 応答形式

正常応答は、コマンドごとの固定応答またはステータス応答とします。

例:

```text
PONG
OK START
OK STOP
OK RESET
OK BYE
STATUS STATE=RUN TEMP=25.4 HUMI=52.1
```

## エラー応答

```text
ERROR UNKNOWN_COMMAND
ERROR INVALID_FORMAT
ERROR INTERNAL_ERROR
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
- `PING`、`GET_STATUS`、`QUIT` は内部状態を変更しない。

## セッション例

```text
Client -> Server: PING
Server -> Client: PONG

Client -> Server: START
Server -> Client: OK START

Client -> Server: GET_STATUS
Server -> Client: STATUS STATE=RUN TEMP=25.4 HUMI=52.1

Client -> Server: QUIT
Server -> Client: OK BYE
```

## Phase 3 実装メモ

Phase 3 のサーバは、このプロトコルに従って実装します。設計書を更新せずに追加コマンドを増やさない方針とします。

