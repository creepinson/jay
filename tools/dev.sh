#!/bin/bash
set -eo pipefail
mkdir build || true

cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=YES \
    -DCMAKE_CXX_COMPILER=clang++ \
    -G Ninja -B build
cmake --build build --target all -j

./build/jay_test
