#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

function pre_install() {
    sudo apt-add-repository -y "ppa:ubuntu-toolchain-r/test"
    sudo apt-get update
}

function install() {
    sudo apt-get "install" "$1"
}
