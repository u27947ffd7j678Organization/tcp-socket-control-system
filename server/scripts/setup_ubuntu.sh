#!/usr/bin/env bash
set -e

sudo apt update
sudo apt install -y \
    build-essential \
    cmake \
    git \
    gdb \
    pkg-config

echo "Ubuntu development environment setup completed."

