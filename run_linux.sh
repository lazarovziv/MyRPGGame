#!/bin/bash

if [ ! command -v <cmake> &> /dev/null ] || [ ! command -v <ninja> &> /dev/null ]; then
  echo "cmake or ninja aren't installed. Install? [y/n]"
  read
  if [ "$REPLY" = "y" ] || [ "$REPLY" = "Y" ]; then
    echo "Installing..."
    sudo apt-get cmake ninja-build -y
  fi
fi

if [ ! dpkg -s libsfml-dev &> /dev/null ]; then
  echo "SFML library isn't installed. Install? [y/n]"
  read
  if [ "$REPLY" = "y" ] || [ "$REPLY" = "Y" ]; then
    echo "Installing..."
    sudo apt-get libsfml-dev -y
  fi
fi

DIR=build
if [ -d "$DIR" ]
then
  echo "Directory already exists. Deleting..."
  rm -rf build
fi

mkdir build
cmake -B build/ -G Ninja
cd build
ninja
./main.out

echo "To rerun, enter build folder and run main.out executable."