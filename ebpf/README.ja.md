# eBPF TCP Monitor

[English version](README.md)

このディレクトリには、Linux 上の TCP 接続を観測する実験的な eBPF ベースの監視ツールを配置しています。

このモニタは TCP サーバや Python クライアントとは独立した監視ツールです。既存の TCP サーバ本体、クライアント、通信プロトコルの挙動は変更しません。

## 現在の監視内容

- `sys_enter_connect` tracepoint へ attach する
- `connect()` システムコールの発生を検知する
- イベント種別、PID、プロセス名を取得する
- BPF Ring Buffer を使ってカーネル空間からユーザー空間へイベントを渡す
- 受信したイベントをターミナルへ表示する

現在の実装では、接続先 IP アドレスやポート番号はまだ取得していません。

## ファイル構成

```text
ebpf/
|-- Makefile
|-- README.md
|-- README.ja.md
|-- tcp_monitor.c
|-- tcp_monitor.bpf.c
`-- tcp_monitor.h
```

## 前提環境

確認対象の環境:

- Ubuntu 24.04 LTS
- clang
- gcc
- make
- libelf-dev
- zlib1g-dev
- pkg-config
- `~/libbpf-bootstrap` に libbpf-bootstrap を clone 済み
- `~/libbpf-bootstrap/examples/c` を事前に build 済み

必要なパッケージの例:

```bash
sudo apt install clang llvm make gcc libelf-dev zlib1g-dev pkg-config
```

先に libbpf-bootstrap を build します。

```bash
cd ~/libbpf-bootstrap/examples/c
make
```

この Makefile は、以下の生成物が存在することを前提にしています。

```text
~/libbpf-bootstrap/examples/c/.output/libbpf/libbpf.a
~/libbpf-bootstrap/examples/c/.output/bpftool/bootstrap/bpftool
~/libbpf-bootstrap/vmlinux.h/include/x86/vmlinux.h
```

## ビルド

```bash
cd ebpf
make
```

ビルドの流れ:

```text
tcp_monitor.bpf.c
  -> clang -target bpf
  -> .output/tcp_monitor.bpf.o
  -> bpftool gen skeleton
  -> .output/tcp_monitor.skel.h
  -> gcc + libbpf + libelf + zlib
  -> tcp_monitor
```

## 実行

eBPF プログラムのロードには、通常 root 権限が必要です。

```bash
sudo ./tcp_monitor
```

出力例:

```text
TCP monitor started. Press Ctrl+C to stop.
type=1 pid=32100 comm=sudo
type=1 pid=496 comm=systemd-resolve
type=1 pid=754 comm=NetworkManager
```

終了するときは `Ctrl+C` を押します。

## クリーン

```bash
make clean
```

## 現在の制約

- ホスト上のすべての `connect()` を取得する
- port `5000` のみを対象にするフィルタは未実装
- 接続先 IP アドレスとポート番号は未表示
- `accept`、`send`、`recv`、`close` の監視は未実装
- TCP サーバ、クライアント、プロトコルは変更しない監視専用ツールとして扱う

## 今後の拡張候補

- `ctx->args[1]` から `sockaddr` を取得する
- `bpf_probe_read_user()` でユーザー空間の `sockaddr` を安全にコピーする
- IPv4 アドレスと接続先ポートをイベントへ追加する
- ユーザー空間で `inet_ntop()` と `ntohs()` を使って表示する
- port `5000` のフィルタリングを追加する
- `send` / `recv` / `close` の監視を追加する
