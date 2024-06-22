#!/bin/bash

docker run --rm -v $(pwd):/workspace --name rpg-compile-container rpg-compile /bin/bash -c "
  cmake -B build/ -G Ninja -DCMAKE_BUILD_TYPE=Release . && \
  cmake --build build/ --config Release && \
  exit"

rm -rf build/bin/CMakeFiles

if [ $(ls -la | wc -l) -ge 1 ]; then
  mv build/MyRPGGame/core/* build/bin/
fi

if [[ $1 == "run" ]]; then
  cd build/bin
  ./main
fi
