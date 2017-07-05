#! /bin/bash
set -e
rm -rf build
mkdir build
cd build
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ../.. && cmake --build .
cd ..
mkdir Debug
cd Debug
cmake -DCMAKE_BUILD_TYPE=Debug ../.. && cmake --build .
cd ..
cd ..