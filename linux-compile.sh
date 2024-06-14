#!/bin/bash

docker run --rm -v $(pwd):/home/compile/MyRPGGame --name rpg-compile-container rpg-compile:1.1 /bin/bash -c "
  cmake -B build/ -G Ninja -DCMAKE_BUILD_TYPE=Release . && \
  cmake --build build/ --config Release && \
  exit"

rm -rf build/bin/CMakeFiles

if [ $(ls -la | wc -l) -ge 1 ]; then
  mv build/MyRPGGame/core/* build/bin/
fi

cd build/bin
./main
