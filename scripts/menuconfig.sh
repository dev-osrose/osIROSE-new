#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

GCOV_VERSION=0
GCC_VERSION=0

declare -a variables=("PROOT=$(pwd)"
                      "GCC_VERSION=5"
                      "GCOV_PATH=/usr/bin/gcov-${GCOV_VERSION}"
                      "LCOV_PATH=${HOME}/usr/bin/lcov"
                      "GENHTML_PATH=${HOME}/usr/bin/genhtml"
                      "PREFIX=${HOME}"
                      "CXX=/usr/bin/g++-${GCC_VERSION}"
                      "CC=/usr/bin/gcc-${GCC_VERSION}"
                      "BUILD_TYPE=Debug")

for f in ${variables[@]}; do
    var=$(echo "$f" | cut -d= -f1)
    val=$(echo "$f" | cut -d= -f2)
    echo "${var}"
    echo "${val}"
done | yad --width=500 \
    --height=300 \
    --title="Build configuration"\
    --text="Configure variables used for compilation" \
    --list \
    --editable \
    --print-all \
    --multiple \
    --column="Variable" \
    --column="Value"\
    | cut -d'|' -f1,2 \
    | sed -e 's/\(.*\)|\(.*\)/export \1=\2/'
exit 0
