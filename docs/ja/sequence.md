# 通信シーケンス

## 通常接続

```text
Client                         Server
  |                              |
  | -------- TCP Connect ------> |
  | <-------- Accept ----------- |
  |                              |
  | -------- PING -------------> |
  | <-------- PONG ------------- |
  |                              |
  | -------- GET_STATUS -------> |
  | <-------- STATUS ... ------- |
  |                              |
  | -------- QUIT -------------> |
  | <-------- OK BYE ----------- |
  |                              |
  | -------- TCP Close --------> |
```

## START / STOP 制御

```text
Client                         Server
  |                              |
  | -------- START -----------> |
  | <-------- OK START -------- |
  |                              |
  | -------- GET_STATUS ------> |
  | <-------- STATUS STATE=RUN |
  |                              |
  | -------- STOP ------------> |
  | <-------- OK STOP --------- |
```

## 未知コマンド

```text
Client                         Server
  |                              |
  | -------- HELLO -----------> |
  | <-------- ERROR UNKNOWN_COMMAND |
```

## RESET の流れ

```text
Client                         Server
  |                              |
  | -------- START -----------> |
  | <-------- OK START -------- |
  |                              |
  | -------- RESET -----------> |
  | <-------- OK RESET -------- |
  |                              |
  | -------- GET_STATUS ------> |
  | <-------- STATUS STATE=STOP |
```

## 実装範囲

この文書は意図する動作を示す設計書です。Phase 2.5 ではソケット実装は追加しません。

