#!/bin/bash
ls /usr/bin/llvm*
exec /usr/bin/llvm-cov-3.7 gcov "$@"
