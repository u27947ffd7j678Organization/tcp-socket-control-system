# TCP Socket Control System

[English README](README.en.md)

Ubuntu 24.04 上で C 言語による TCP/IP 通信制御システムを段階的に作成する学習用プロジェクトです。

現在は **Phase 2.5: リポジトリ公開準備** の段階です。TCP ソケット通信の実装はまだ追加していません。

## プロジェクト概要

このリポジトリでは、産業機器や組み込み通信を意識した小さな通信制御システムを題材にしています。

```text
Windows PC Client
        |
        | TCP/IP
        v
Ubuntu Linux TCP Server
```

最初の実装対象は Ubuntu 上で動作する TCP サーバです。クライアントから行単位のコマンドを受信し、内部状態を更新して、テキスト応答を返す構成を予定しています。

将来的には Windows GUI クライアント、ログ保存、設定ファイル、単体テスト、GitHub Actions、CAN またはセンサシミュレーションへの拡張を行います。

## 学習目的

- C 言語で TCP/IP ソケットプログラミングを段階的に学ぶ。
- 実装前に通信プロトコルを設計する。
- 構成図、シーケンス、状態遷移を Markdown で残す。
- 他の開発者が clone して続けやすい構成にする。
- 英語の実装仕様と日本語の公開説明を並行して管理する。

## 開発環境

- OS: Ubuntu 24.04
- Language: C17
- Build system: CMake
- Compiler: gcc
- Debugger: gdb
- IDE: VS Code with Remote SSH
- Version control: Git

## リポジトリ構成

```text
tcp-socket-control-system/
|-- docs/
|   |-- en/
|   |-- ja/
|   `-- images/
|-- include/
|-- src/
|-- tests/
|-- scripts/
|   `-- setup_ubuntu.sh
|-- CMakeLists.txt
|-- README.md
|-- README.en.md
|-- LICENSE
`-- .gitignore
```

## Ubuntu セットアップ

Ubuntu 24.04 上で以下を実行します。

```bash
chmod +x scripts/setup_ubuntu.sh
./scripts/setup_ubuntu.sh
```

このスクリプトは以下をインストールします。

- `build-essential`
- `cmake`
- `git`
- `gdb`
- `pkg-config`

## ビルド手順

現時点では実行ファイルはまだ実装していません。CMake プロジェクトの構成確認として以下を実行します。

```bash
cmake -S . -B build
cmake --build build
```

## ドキュメント

- 英語版の実装仕様: [docs/en/](docs/en/)
- 日本語版の公開説明: [docs/ja/](docs/ja/)

主なドキュメント:

- [システム概要](docs/ja/system_overview.md)
- [通信プロトコル仕様](docs/ja/protocol_spec.md)
- [通信シーケンス](docs/ja/sequence.md)
- [状態遷移設計](docs/ja/state_transition.md)
- [開発ログ](docs/ja/development_log.md)

## ロードマップ

- [x] Phase 1: プロジェクトの土台
- [x] Phase 2: TCP/IP システム設計書
- [x] Phase 2.5: リポジトリ公開準備
- [ ] Phase 3: TCP サーバ実装
- [ ] Phase 4: TCP クライアント
- [ ] Phase 5: Windows GUI
- [ ] Phase 6: ログ保存・設定ファイル対応
- [ ] Phase 7: GitHub Actions・単体テスト
- [ ] Phase 8: ポートフォリオ公開整備

## 現在の状態

このリポジトリは、設計を先に整えた C ネットワーク学習プロジェクトとして公開できる状態です。Phase 3 では `docs/en/protocol_spec.md` の仕様に沿って最初の TCP サーバを実装します。
