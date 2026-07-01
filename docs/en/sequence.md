# Communication Sequences

## Normal Connection

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

## Start / Stop Control

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

## Unknown Command

```text
Client                         Server
  |                              |
  | -------- HELLO -----------> |
  | <-------- ERROR UNKNOWN_COMMAND |
```

## Reset Flow

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

## Implementation Boundary

These sequences describe the intended behavior only. Phase 2.5 does not add socket code.

