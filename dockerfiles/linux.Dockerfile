# docker image for the build environment
FROM ubuntu:latest

RUN apt update
# last 2 lines are necessary packages for using SFML
RUN apt install -y cmake build-essential ca-certificates wget ninja-build gcc g++ unzip git clang \
           libfreetype-dev libx11-dev libxrandr-dev libudev-dev libopengl-dev libflac-dev libogg-dev libvorbis-dev \
           libvorbisenc2 libvorbisfile3 libopenal-dev libblis-pthread-dev libopengl-dev libglew-dev libxcursor-dev

# using a local user for permissions when mounting the local workspace into the container
ARG UNAME=compile
ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID -o $UNAME
RUN useradd -m -u $UID -g $GID -o -s /bin/bash $UNAME
USER $UNAME

WORKDIR /home/$UNAME

RUN git clone https://github.com/lazarovziv/MyRPGGame.git

# setting env variables for compilation
ENV CXX=/usr/bin/clang++
ENV CC=/usr/bin/clang

WORKDIR /home/$UNAME/MyRPGGame
# initializing all submodules in case we're not mounting a workspace
RUN git submodule update --init --recursive
