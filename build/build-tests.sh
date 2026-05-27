#!/usr/bin/env bash
# build-tests.sh — собирает и запускает tests/tests.cpp
#
# Требование: libraylib.a уже собрана (`make config=debug_x64`).
# Тестам не нужно окно, но raylib линкуется для TraceLog/Vector2Normalize.

set -euo pipefail
cd "$(dirname "$0")/.."

if [ ! -f bin/Debug/libraylib.a ]; then
    echo "[tests] libraylib.a not found at bin/Debug/. Build first:" >&2
    echo "        make config=debug_x64" >&2
    exit 1
fi

echo "[tests] compiling..."
g++ -std=c++17 -O0 -g -Wall -Wno-unused-variable \
    -I src \
    -I build/external/raylib-master/src \
    tests/tests.cpp \
    bin/Debug/libraylib.a \
    -lpthread -ldl -lm -lrt -lX11 \
    -o bin/Debug/run-tests

echo "[tests] running..."
./bin/Debug/run-tests
