#!/bin/bash

if [ "$1" == "run" ]; then
  echo "Compiling and running..."
#  docker-compose -f dockerfiles/linux-start-docker-compose.yml up
  docker run --rm -v $(pwd):/workspace -v /tmp/.X11-unix:/tmp.X11-unix -v /dev/shb:/dev/shm \
    -e DISPLAY=$DISPLAY --net host \
    --device /dev/input --device /dev/dri --name rpg zivlazarov/rpg:1.1 \
    bash -c "cmake -B build/ -G \"Ninja\" . && cmake --build build/ && cd /workspace/build/bin && ./main"

elif [ "$1" == "test" ]; then
  echo "Compiling and running tests..."
#  docker-compose -f dockerfiles/linux-compile-docker-compose.yml up
  docker run --rm -v $(pwd):/workspace -v /tmp/.X11-unix:/tmp.X11-unix -v /dev/shb:/dev/shm \
    -e DISPLAY=$DISPLAY --net host --name rpg zivlazarov/rpg:1.1 \
    bash -c "cmake -B build/ -G \"Ninja\" . && cmake --build build/ && cd /workspace/build/bin && ./tests"

elif [ "$1" == "vulkan" ]; then
  if [ "$2" == "run" ]; then
    echo "Compiling and running Vulkan components..."
    docker run --rm -v $(pwd):/workspace -v /tmp/.X11-unix:/tmp.X11-unix -v /dev/shm:/dev/shm \
      -e DISPLAY=$DISPLAY -w /workspace/MyRPGGame/vulkan --net host \
      --device /dev/input --device /dev/dri --name rpg zivlazarov/rpg:1.2 \
      bash -c "cmake -D GLFW_BUILD_X11=1 -D GLFW_BUILD_WAYLAND=0 -B build/ -G \"Ninja\" . && cmake --build build/ \
      && cd build/bin && ./vulkan"
  else
    echo "Compiling Vulkan components..."
    #  docker-compose -f dockerfiles/linux-compile-docker-compose.yml up
    docker run --rm -v $(pwd):/workspace -v /tmp/.X11-unix:/tmp.X11-unix -e DISPLAY=$DISPLAY \
      -w /workspace/MyRPGGame/vulkan --net host --name rpg zivlazarov/rpg:1.2 \
      bash -c "cmake -D GLFW_BUILD_X11=1 -D GLFW_BUILD_WAYLAND=0 -B build/ -G \"Ninja\" . && cmake --build build/"
  fi
fi
