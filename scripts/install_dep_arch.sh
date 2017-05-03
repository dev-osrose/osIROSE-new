#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

function pre_install() {
    return 0
}

function install() {
    sudo pacman -S "$1"
}

