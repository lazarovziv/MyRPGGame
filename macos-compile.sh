#!/bin/bash

DOCKER_IMAGE="zivlazarov/rpg:1.3"

COMMAND=$1
FIRST_OPTION=$2
SECOND_OPTION=$3

DOCKER_OPTIONS="--rm -v $(pwd):/workspace -v /tmp/.X11-unix:/tmp/.X11-unix:rw \
      -v $HOME/.Xauthority:/home/ubuntu/.Xauthority:rw --ipc=host \
      -e DISPLAY="host.docker.internal:0" --net x11-network \
      --pid=host --user ubuntu --name rpg --cpus=4"

if [ "$SECOND_OPTION" != "" ]; then
  SECOND_OPTION="--config $SECOND_OPTION"
fi

if [ "$COMMAND" == "run" ]; then
  echo "Compiling and running..."
#  docker-compose -f dockerfiles/linux-start-docker-compose.yml up
  docker run $DOCKER_OPTIONS $DOCKER_IMAGE \
    bash -c "cmake -B build/ -G \"Ninja\" . && cmake --build build/ && cd /workspace/build/bin && ./main"

elif [ "$COMMAND" == "test" ]; then
  echo "Compiling and running tests..."
#  docker-compose -f dockerfiles/linux-compile-docker-compose.yml up
  docker run $DOCKER_OPTIONS $DOCKER_IMAGE \
    bash -c "cmake -B build/ -G \"Ninja\" . && cmake --build build/ $SECOND_OPTION && \
     cd /workspace/build/bin && ./tests"

elif [ "$COMMAND" == "vulkan" ]; then
  if [ "$FIRST_OPTION" == "run" ]; then
    echo "Compiling and running Vulkan components..."
    docker run $DOCKER_OPTIONS -w /workspace/MyRPGGame/vulkan $DOCKER_IMAGE \
      bash -c "echo \"Compiling shaders...\" && glslc shaders/shader.vert -o shaders/vert.spv && \
      glslc shaders/shader.frag -o shaders/frag.spv && echo \"Shaders compiled!\" && \
      cmake -D GLFW_BUILD_X11=1 -D GLFW_BUILD_WAYLAND=0 -B build/ -G \"Ninja\" . && \
      cmake --build build/ $SECOND_OPTION && cd build/bin && ./vulkan"
  else
    echo "Compiling Vulkan components..."
    #  docker-compose -f dockerfiles/linux-compile-docker-compose.yml up
    docker run $DOCKER_OPTIONS $DOCKER_IMAGE \
      bash -c "cmake -D GLFW_BUILD_X11=1 -D GLFW_BUILD_WAYLAND=0 -B build/ -G \"Ninja\" . && cmake --build build/"
  fi
fi
