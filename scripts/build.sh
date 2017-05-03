#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

mkdir --parents build

if [[ -e build/deps ]]
then
    scripts/check_deps.sh
    touch build/deps
fi

source "scripts/default-env.sh"

cd build
cmake -DENABLE_TESTING=OFF\
      -DENABLE_TESTING_COVERAGE=OFF\
      -DBUILD_PROTOBUF=OFF\
      -DBUILD_MYSQLPP=OFF\
      ..

make
