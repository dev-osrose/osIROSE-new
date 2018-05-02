#!/bin/sh

if [ "$SUITE" = "coverage" ]; then
  cmake -GNinja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DWITH_CRASH_REPORTS=OFF -DWITH_COVERAGE_REPORTS=ON .. && cmake --build . -- -j 4 && cmake --build . --target coverage && coveralls-lcov coverage.info.cleaned
elif [ "$SUITE" = "tests" ]; then
  cmake -GNinja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DWITH_CRASH_REPORTS=OFF .. && cmake --build . -- -j 4 && ctest --output-on-failure
else
  cmake -GNinja -DCMAKE_CXX_COMPILER_LAUNCHER=ccache -DWITH_CRASH_REPORTS=OFF -DBUILD_TESTS=OFF .. && cmake --build . -- -j 4
fi
