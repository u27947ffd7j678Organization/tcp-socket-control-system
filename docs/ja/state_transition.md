# 状態遷移設計

## 目的

この文書は、通信プロトコルで使用するサーバの状態モデルを定義します。

最初の実装では、TCP サーバの理解とテストをしやすくするため、状態は 2 つだけにします。

## 初期状態

```text
STOP
```

## 状態

| 状態 | 意味 |
|---|---|
| `STOP` | サーバが停止または待機している。 |
| `RUN` | サーバが稼働している。 |

## 状態遷移

```text
STOP -- START --> RUN
RUN  -- STOP  --> STOP
RUN  -- RESET --> STOP
STOP -- RESET --> STOP
```

## 状態遷移表

| 現在状態 | コマンド | 次状態 | 応答 |
|---|---|---|---|
| `STOP` | `START` | `RUN` | `OK START` |
| `RUN` | `START` | `RUN` | `OK START` |
| `RUN` | `STOP` | `STOP` | `OK STOP` |
| `STOP` | `STOP` | `STOP` | `OK STOP` |
| `RUN` | `RESET` | `STOP` | `OK RESET` |
| `STOP` | `RESET` | `STOP` | `OK RESET` |

## 状態を変更しないコマンド

以下のコマンドは状態を変更しません。

- `PING`
- `GET_STATUS`
- `QUIT`

## 不正コマンド

未知コマンドや不正形式のコマンドでは状態を変更しません。

例:

```text
HELLO -> ERROR UNKNOWN_COMMAND
empty line -> ERROR INVALID_FORMAT
```

