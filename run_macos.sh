#!/bin/zsh

# remove mac os compiler comment
sed -i '' -e 's/\#//g' CMakeLists.txt

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