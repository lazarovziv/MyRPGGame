#!/bin/bash

if [ "$1" == "start" ]; then
  echo "Compiling and Starting..."
#  docker-compose -f dockerfiles/linux-start-docker-compose.yml up
  docker run --rm -v $(pwd):/workspace -v /tmp/.X11-unix:/tmp.X11-unix -e DISPLAY=$DISPLAY --net host --name rpg rpg \
    bash -c "cmake -B build/ -G \"Ninja\" . && cmake --build build/ && cd /workspace/build/bin && ./main"
else
  echo "Compiling..."
#  docker-compose -f dockerfiles/linux-compile-docker-compose.yml up
  docker run --rm -v $(pwd):/workspace -v /tmp/.X11-unix:/tmp.X11-unix -e DISPLAY=$DISPLAY --net host --name rpg rpg \
    bash -c "cmake -B build/ -G \"Ninja\" . && cmake --build build/"
fi
