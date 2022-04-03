#! /bin/sh
# Run as
# $ build.sh [release|debug]

cmake -DCMAKE_BUILD_TYPE=${1:-release} -S . -B build
cmake --build build