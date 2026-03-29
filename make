#!/bin/bash

if [[ "$1" == "debug" ]]; then
   cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug
   cmake --build build/debug --target install
   valgrind ./build/debug/tineye ./test_files dang ~/.vim/tineye
   exit 0
fi

cmake -B build/release
cmake --build build/release --target install
./bin/tineye ./test_files dang ~/.vim/tineye
