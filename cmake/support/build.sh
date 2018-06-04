#!/usr/bin/env bash
set -euo pipefail
IFS=$'\n\t'

mkdir --parents build

if [[ -e build/deps ]]
then
    scripts/check_deps.sh
    touch build/deps
fi

if [[ -e "scripts/env.sh" ]]
then
    source "scripts/env.sh"
else
    source "scripts/default-env.sh"
fi

cd build
cmake -DENABLE_TESTING=OFF\
      -DENABLE_TESTING_COVERAGE=OFF\
      -DBUILD_MYSQLPP=OFF\
      ..

make
