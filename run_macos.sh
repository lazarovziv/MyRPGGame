#!/bin/zsh

cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release .
cmake --build build/ --config Release

echo "Run? (y/n)"
read ANSWER

if [ "$ANSWER"="y" ]; then
  cd build/Release
  ./main
fi
