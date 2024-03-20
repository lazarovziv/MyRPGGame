#!/bin/zsh

cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release .
if [ $? != 0 ]; then
  echo "Compilation failed. Exiting..."
  exit 1
fi

cmake --build build/ --config Release
if [ $? != 0 ]; then
  echo "Compilation failed. Exiting..."
  exit 1
fi

cd build/Release
./main
